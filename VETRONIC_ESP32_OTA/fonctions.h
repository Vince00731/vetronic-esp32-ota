
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Vide le buffer de réception du port COM
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ClearBufferRxCom(void) {
  while (COM->available()) {
    COM->read();  // Vide le buffer de réception
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Envoi une commande à la borne et retourne la réponse sous forme de string brute
/////////////////////////////////////////////////////////////////////////////////////////////////////
String Read_Evse(String commande) {
  String str_reponse = "";
  uint8_t step = ATTENTE_REQUETE;
  ClearBufferRxCom();
  COM->flush();
  COM->print(commande);  // now send to UART:
  COM->write('\r');      // Nécessaire pour que la borne prenne la commande
  if (debug) Serial.print("Requete = ");
  if (debug) Serial.println(commande);  // send to Serial 2
  cpt_buffer_com = 0;
  step = READ_EVSE;
  previousTime = millis();  // Start Tempo TimeOut COM

  while (step == READ_EVSE) {
    if (COM->available()) {  // Test if COM answer ?
      while (COM->available()) {
        char c = COM->read();  // read char from UART
        buffer_com[cpt_buffer_com] = c;
        if (cpt_buffer_com < BUFFERSIZE - 1) cpt_buffer_com++;
      }
      previousTime = millis();  // Restart timeout à chaque tour de boucle
    } else {
      currentTime = millis();  // Tempo d'attente sur reponse COM
      if ((currentTime - previousTime) >= timeoutTime) {
        buffer_com[cpt_buffer_com] = 0;
        step = END;
      }
    }
  }

  if (cpt_buffer_com == 0) str_reponse = String("Pas de reponse du Vetronic");
  else str_reponse = String(buffer_com);

  if (debug) Serial.println("");
  if (debug) Serial.println("Received from COM =");
  if (debug) Serial.write((const unsigned char*)buffer_com, cpt_buffer_com);  // send to Serial:
  if (debug) Serial.println("");
  return str_reponse;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Retourne les valeurs séparées par des séparateurs
/////////////////////////////////////////////////////////////////////////////////////////////////////
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Convertion des caractères
/////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char h2int(char c) {
  if (c >= '0' && c <= '9') {
    return ((unsigned char)c - '0');
  }
  if (c >= 'a' && c <= 'f') {
    return ((unsigned char)c - 'a' + 10);
  }
  if (c >= 'A' && c <= 'F') {
    return ((unsigned char)c - 'A' + 10);
  }
  return (0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Décodage de l'URL
/////////////////////////////////////////////////////////////////////////////////////////////////////
String urldecode(String str) {

  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == '+') {
      encodedString += ' ';
    } else if (c == '%') {
      i++;
      code0 = str.charAt(i);
      i++;
      code1 = str.charAt(i);
      c = (h2int(code0) << 4) | h2int(code1);
      encodedString += c;
    } else {

      encodedString += c;
    }
  }
  return encodedString;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Creation de la reponse XML
/////////////////////////////////////////////////////////////////////////////////////////////////////
String DoRepXml(String req, String erreur, String reponse) {

  int Index_Start = 0;

  String message = "";
  String state = "";
  String state_txt = "";
  String courant_charge = "";
  String tension = "";
  String courant_maximal = "";

  String reponse_temporaire = "";
  String srep = "<?xml version='1.0' encoding='UTF-8'?>\r\n";

  state_txt = "NaN";
  state = "NaN";
  courant_charge = "NaN";
  tension = "NaN";
  courant_maximal = "NaN";

  req.trim();
  if (reponse == "") {
    reponse = String("ERROR 444");
    message = String("ERROR 444 pas de réponse de la borne");
    erreur = String("444");
    String srep = "<?xml version='1.0' encoding='UTF-8'?>\r\n";
  } else if (erreur == "200") {    // teste si la reponse vetronic = OK = 200
    reponse.trim();                // Supprime les espaces au début et à la fin de la reponse
    reponse_temporaire = reponse;  // Copie la réponse

    //*************************************************************************************************
    // Commande HELP ou LIST
    //*************************************************************************************************
    if ((req.indexOf("help") != -1) or (req.indexOf("list") != -1)) {
      message = String("Requete OK et reponse.Cde. " + req + " : Borne Vetronic OK");
    }

    //*************************************************************************************************
    // Commande evse_state + $GG*B2 en une seule requête
    //*************************************************************************************************
    else if (req.indexOf("evse_state + $GG*B2") != -1) {
      reponse_temporaire.replace("\r\n", "");  // Suppression des caractères CR et LF
      reponse_temporaire.replace("\r", "");    // Suppression des caractères CR
      reponse_temporaire.replace("\n", "");    // Suppression des caractères LF
      if (reponse_temporaire.indexOf("EVSE state :") != -1) {
        Index_Start = reponse_temporaire.indexOf("EVSE state :");                               // Recherche le début de la zone utile
        reponse_temporaire = reponse_temporaire.substring(Index_Start + 13, Index_Start + 14);  // Isole le chiffre qui nous intéresse
        state = reponse_temporaire;
        if(state == "0") state_txt = "Câble déconnecté";
        else if(state == "1") state_txt = "Câble connecté";
        else if(state == "2") state_txt = "Véhicule en charge";
        else state_txt = "Status inconnu !";
        reponse_temporaire = reponse;                      // Recharge la réponse de départ
        if (reponse.indexOf("$OK") != -1) {                 // test si le vetronic reponde $OK
          reponse_temporaire.replace("\r\n", "");           // Suppression des caractères CR et LF
          reponse_temporaire.replace("\r", "");             // Suppression des caractères CR
          reponse_temporaire.replace("\n", "");             // Suppression des caractères LF
          Index_Start = reponse_temporaire.indexOf("$OK");  // Recherche le début de la zone utile
          reponse_temporaire.remove(0, Index_Start + 4);    // Supprime le début de la réponse
          reponse_temporaire.replace("wallbox$", "");       // supprime wallbox$ de la reponse

          courant_charge = getValue(reponse_temporaire, ' ', 0);   // Courant absorbé par le véhicule
          tension = getValue(reponse_temporaire, ' ', 1);          // Tension secteur
          courant_maximal = getValue(reponse_temporaire, ' ', 2);  // Courant maximal autorisé

          message = String("Requete OK et reponse.Cde. " + req + " : Borne Vetronic OK");  // message = message OK et reponse Vetronic OK

        } else {                                                                                 // Vetronic n'a pas repondu $OK a la Cde "$SC x"
          state = "NaN";
          message = String("Requete OK mais reponse.Cde. " + req + " : Borne Vetronic PAS OK");  // Requete OK mais Vetronic repond PAS OK sur la Commande                                                 // supprime la commande de la reponse
          erreur = String("400");
        }
      } else {                                                                                 // Vetronic n'a pas repondu $OK a la Cde "$SC x"
        message = String("Requete OK mais reponse.Cde. " + req + " : Borne Vetronic PAS OK");  // Requete OK mais Vetronix repond PAS OK sur la Commande                                                 // supprime la commande de la reponse
        erreur = String("400");
      }
    }

    //*************************************************************************************************
    // Commande $GG*B2 = Status de la charge
    //*************************************************************************************************
    else if (req.indexOf("$GG*B2") != -1) {
      if (reponse.indexOf("$OK") != -1) {                 // test si le vetronic reponde $OK
        reponse_temporaire.replace("\r\n", "");           // Suppression des caractères CR et LF
        reponse_temporaire.replace("\r", "");             // Suppression des caractères CR
        reponse_temporaire.replace("\n", "");             // Suppression des caractères LF
        Index_Start = reponse_temporaire.indexOf("$OK");  // Recherche le début de la zone utile
        reponse_temporaire.remove(0, Index_Start + 4);    // Supprime le début de la réponse qui peut contenir du texte suivant le contexte de la borne
        reponse_temporaire.replace("wallbox$", "");       // supprime wallbox$ de la reponse

        message = String("Requete OK et reponse.Cde. " + req + " : Borne Vetronic OK");  // message = message OK et reponse Vetronic OK

        courant_charge = getValue(reponse_temporaire, ' ', 0);   // Courant absorbé par le véhicule
        tension = getValue(reponse_temporaire, ' ', 1);          // Tension secteur
        courant_maximal = getValue(reponse_temporaire, ' ', 2);  // Courant maximal autorisé

        // La conversion en Ampère sera faite dans Jeedom, c'est plus simple pour gérer les virgules.
        /*// La Borne retourne la puissance en milliampère > convert en ampère
        if (courant_charge != "") {
          int amp = courant_charge.toInt();  // divise la charge rendu par Vetronic par 1000 ex: 19000 = 19amp
          if (amp >= 1000) amp = amp / 1000;
          courant_charge = String(amp);
        }
        if (courant_maximal != "") {
          int amp = courant_maximal.toInt();  // divise la charge rendu par Vetronic par 1000 ex: 19000 = 19amp
          if (amp >= 1000) amp = amp / 1000;
          courant_maximal = String(amp);
        }
        */
      } else {                                                                                 // Vetronic n'a pas repondu $OK a la Cde "$GG*B2"
        message = String("Requete OK mais reponse.Cde. " + req + " : Borne Vetronic PAS OK");  // Requete OK mais Vetronix repond PAS OK sur la Commande
        reponse_temporaire.replace(req, "");                                                   // supprime la commande de la reponse
        erreur = String("400");
      }
    }

    //*************************************************************************************************
    // Commande $SC = Limite du courant de charge
    //*************************************************************************************************
    else if (req.indexOf("$SC") != -1) {
      if (reponse_temporaire.indexOf(req) != -1) {
        message = String("Requete OK et reponse.Cde. " + req + " : Borne Vetronic OK");        // message = message OK et reponse Vetronic OK
      } else {                                                                                 // Vetronic n'a pas repondu $OK a la Cde "$SC x"
        message = String("Requete OK mais reponse.Cde. " + req + " : Borne Vetronic PAS OK");  // Requete OK mais Vetronix repond PAS OK sur la Commande                                                 // supprime la commande de la reponse
        erreur = String("400");
      }
    }
    //*************************************************************************************************
    // Commande EVSE_STATE
    //*************************************************************************************************
    else if (req.indexOf("evse_state") != -1) {
      reponse_temporaire.replace("\r\n", "");  // Suppression des caractères CR et LF
      reponse_temporaire.replace("\r", "");    // Suppression des caractères CR
      reponse_temporaire.replace("\n", "");    // Suppression des caractères LF
      if (reponse_temporaire.indexOf("EVSE state :") != -1) {
        Index_Start = reponse_temporaire.indexOf("EVSE state :");                               // Recherche le début de la zone utile
        reponse_temporaire = reponse_temporaire.substring(Index_Start + 13, Index_Start + 14);  // Isole le chiffre qui nous intéresse
        state = reponse_temporaire;
        if(state == "0") state_txt = "Câble déconnecté";
        else if(state == "1") state_txt = "Câble connecté";
        else if(state == "2") state_txt = "Véhicule en charge";
        else state_txt = "Status inconnu !";
        message = String("Requete OK et reponse.Cde. " + req + " : Borne Vetronic OK");        // message = message OK et reponse Vetronic OK
      } else {                                                                                 // Vetronic n'a pas repondu $OK a la Cde "$SC x"
        message = String("Requete OK mais reponse.Cde. " + req + " : Borne Vetronic PAS OK");  // Requete OK mais Vetronix repond PAS OK sur la Commande                                                 // supprime la commande de la reponse
        erreur = String("400");
      }
    }
    //*************************************************************************************************
    // Commande inconnue
    //*************************************************************************************************
    else {
      message = String("Requete OK et reponse.Cde." + req + " : Inconnue de l'ESP32");  // message = message OK et reponse Vetronic OK
    }
  }
  //*************************************************************************************************
  // Erreur dans la requête
  //*************************************************************************************************
  else {                                                       // Error la requete n'est pas OK
    message = String("Erreur: " + erreur + " -- " + reponse);  // detail de l'erreur
  }
  srep += "<vetronic><error>" + erreur + "</error><req>" + req + "</req><reponse_brute>" + reponse + "</reponse_brute><message>" + message + "</message><status>" + state + "</status><status_txt>" + state_txt + "</status_txt><courant_charge>" + courant_charge + "</courant_charge><tension>" + tension + "</tension><courant_maximal>" + courant_maximal + "</courant_maximal></vetronic>\r\n";
  return srep;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Envoi de la réponse TCP
/////////////////////////////////////////////////////////////////////////////////////////////////////
void SendResponseHead(String reponse) {

  String resp = "HTTP/1.1 200 OK\r\n";
  resp += "Content-type: text/xml\r\n";
  resp += "Access-Control-Allow-Origin: *\r\n";
  resp += "Access-Control-Expose-Headers: Access-Control-Allow-Origin\r\n";
  resp += "Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept\r\n";
  resp += "Cache-Control: no-cache, no-store, must-revalidate\r\n";
  resp += "Connection: close\r\n\r\n";
  resp += reponse;
  TCPClient.println(resp);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Test de la requête TCP
/////////////////////////////////////////////////////////////////////////////////////////////////////
String TestIfReqValide(String str) {

  str = urldecode(str);
  if ((str.indexOf("/favicon.ico") != -1) || (str.indexOf("GET /") == -1)) {
    str = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE \ HTML>\r\n<html>\r\n</html>\r\n";
    TCPClient.println(str);
    TCPClient.stop();
    return "";
  }
  String Req;
  int index1, index2;
  index1 = str.indexOf("GET /") + 5;
  index2 = str.indexOf("HTTP/1.1");
  Req = str.substring(index1, index2);
  return Req;
}
