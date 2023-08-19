/////////////////////////////////////////////////////////////////////////////////////////////////////
// Connexion de la Borne VETRONIC WB-01 par son UART via un ESP32
// Le firmware de la borne doit être 1.5C ou supérieure
//
// Utilise le Port Série Com2 du ESP32 : voir la configuration des pin dans config.h
// Utilise le Port TCP spécifié dan config.h pour le retour au format XML
// Utilise le Port 80 pour la gestion du WiFi et la mise à jour du firmware de l'ESP 32 par OTA
//
// Le retour en XML se fait par simple appel http en ajoutant la commande de la borne à la fin
// Appels de requête = http://IP-WIFI:PORT_TCP/commande-Vetronic
// Si il n'y a pas de commande spécifiée, le module réalise une interrogation générique de la borne
//
// Auteur: Vincent ROBERT avec l'aide de Christian Dingean (cdlog2@hotmail.com)
// vrelectronique@gmail.com
// Version 1.1 du 19/08/2023
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "config.h"
#include "json_pages.h"
#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <WiFiClient.h>

// Port série COM2
HardwareSerial Serial_two(2);
HardwareSerial* COM = &Serial_two;

// Déclarations des variables globales
char buffer_tcp[BUFFERSIZE];
uint16_t cpt_buffer_tcp = 0;
char buffer_com[BUFFERSIZE];
uint16_t cpt_buffer_com = 0;
uint8_t step_tcp = ATTENTE_REQUETE;
unsigned long currentTime = 0;
unsigned long previousTime = 0;

// Portail AutoConnect
WebServer server;
AutoConnect portal(server);
AutoConnectConfig config;
AutoConnectAux page_results;
AutoConnectAux page_root;
String str_mem_commande = "help";

// Server TCP pour les requêtes en XML
WiFiServer server_2(SERIAL2_TCP_PORT);
WiFiServer* server_tcp = &server_2;
WiFiClient TCPClient;

// Fonctions nécessaires au fonctionnement du programme
#include "Fonctions.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonction appellée lors de l'affichage de la page Results
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AuxResults(AutoConnectAux& aux, PageArgument& args) {
  String str_value = "";
  String str_commande = "";
  str_commande = String(args.arg("commande"));
  str_mem_commande = str_commande;  // Sauvegarde de la commande
  str_value = Read_Evse(str_commande);
  str_value.replace("\r\n", "<br>");                                      // Remplace les saut de ligne TELNET par des saut de ligne HTML
  str_value.replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");  // Remplace les tabulations TELNET par des espaces HTML
  aux["results"].as<AutoConnectText>().value = "<br>" + str_value;
  return String();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonction appellée lors de l'affichage de la page PageRoot
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AuxPageRoot(AutoConnectAux& aux, PageArgument& args) {
  aux["commande"].as<AutoConnectInput>().value = str_mem_commande;
  aux["content"].as<AutoConnectText>().value = "Pour un retour en XML, utiliser la requête suivante : <a href= \"http://" + WiFi.localIP().toString() + ":" + String(SERIAL2_TCP_PORT) + "\">http://" + WiFi.localIP().toString() + ":" + String(SERIAL2_TCP_PORT) + " </a>/<i>commande_Vetronic</i> <br>";
  aux["version"].as<AutoConnectText>().value = "<h5>VETRONIC_ESP32_OTA V" + String(VERSION) + "</h5>";
  return String();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation de l'ESP 32
/////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

  // Port série de l'ESP sur chip USB
  Serial.begin(115200);
  Serial.setRxBufferSize(BUFFERSIZE);
  delay(500);

  // Port série COM vers la borne
  COM->begin(UART_BAUD2, SERIAL_PARAM2, SERIAL2_RXPIN, SERIAL2_TXPIN);
  COM->setRxBufferSize(BUFFERSIZE);
  COM->write('\r');  // Lance le prompt sur la borne VETRONIC
  delay(100);
  ClearBufferRxCom();
  Serial.println("VETRONIC_ESP32_OTA " + String(VERSION));

  // Configuration du portail web AutoConnect
  config.ota = AC_OTA_BUILTIN;
  config.apid = "VETRONIC_ESP32_OTA";     // SSID en mode AP
  config.apip = IPAddress(172, 0, 0, 1);  // Adresse IP en mode AP
  portal.config(config);

  // Déclaration des pages Web
  page_results.load(PAGE_RESULTS);
  page_root.load(PAGE_ROOT);
  portal.join({ page_results, page_root });
  portal.on("/results", AuxResults);
  portal.on("/", AuxPageRoot);

  // Démmarage du portail AutoConnect
  if (portal.begin()) {
    Serial.println("Web server started, IP:" + WiFi.localIP().toString());
  } else {
    Serial.println("Connection failed.");
    while (true) { yield(); }
  }
  // Démmarage du server TCP
  Serial.println("Starting TCP server");
  server_tcp->begin();  // start TCP server
  server_tcp->setNoDelay(true);

  // Initialisation des machines à état
  step_tcp = ATTENTE_REQUETE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Boucle principale
/////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  static String str = "";
  static String str_com = "";
  static String str2_com = "";

  // En cas de blocage du WiFi, redémarrage du module ESP32
  if (WiFi.status() == WL_IDLE_STATUS) {
    ESP.restart();
    delay(1000);
  }

  switch (step_tcp) {
    /////////////
    case ATTENTE_REQUETE:
      portal.handleClient();  // Si on n'est pas en train de traiter une requête TCP, alors on gère le portail http
      if (server_tcp->hasClient()) {
        if (!TCPClient || !TCPClient.connected()) {
          if (TCPClient) TCPClient.stop();
          TCPClient = server_tcp->available();
          if (debug) Serial.println("New client for COM");
        }
      }
      if (TCPClient && TCPClient.available()) {  //find if Client available
        cpt_buffer_tcp = 0;
        while (TCPClient.available()) {
          buffer_tcp[cpt_buffer_tcp] = TCPClient.read();  // read char from Tcp client
          if (cpt_buffer_tcp < BUFFERSIZE - 1) cpt_buffer_tcp++;
        }
        buffer_tcp[cpt_buffer_tcp] = 0;
        TCPClient.flush();

        /////////////////////////
        // Test la Requete reçue
        /////////////////////////
        str = TestIfReqValide(String(buffer_tcp));
        if (str == "") {
          step_tcp = ATTENTE_REQUETE;
        } else step_tcp = READ_EVSE;
      }
      break;

    /////////////
    case READ_EVSE:
      str_com = "";
      // Si il n'y a pas de commande dans la requête (lu comme un espace)
      if (str == " ") {
        str = "evse_state + $GG*B2";  // Alors on fait une double lecture des status
        str_com = Read_Evse("evse_state");
        str2_com = Read_Evse("$GG*B2");
        str_com = str_com + str2_com;

      // Sinon on fait une lecture classique
      } else {
        str_com = Read_Evse(str);        
      }
      step_tcp = ENVOI_REQ;
      break;

    ////////////////
    case ENVOI_REQ:
      if (str_com.indexOf("Pas de reponse du Vetronic") != -1) str = DoRepXml(str, "444", "");
      else str = DoRepXml(str, "200", str_com);
      SendResponseHead(str);  // renvoie la reponse COM2 to TCP WIFI
      TCPClient.stop();
      step_tcp = ATTENTE_REQUETE;
      break;
  }
}
