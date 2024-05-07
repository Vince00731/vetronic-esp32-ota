#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <String.h>

/*************************  Constante ****************************/
#define VERSION "v2.2"
#define ATTENTE_REQUETE 1
#define READ_EVSE 2
#define ENVOI_REQ 3
#define UPDATE_EVSE 4
#define UPDATE_JEEDOM 5
#define END 3
#define BUFFERSIZE 1024

/*************************  COM Port 2 ****************************/
#define UART_BAUD2 115200         // Baudrate UART2
#define SERIAL_PARAM2 SERIAL_8N1  // Data/Parity/Stop UART2
#define SERIAL2_RXPIN 15          // receive Pin UART2
#define SERIAL2_TXPIN 4           // transmit Pin UART2
/**********************  Port d'écoute TCP ************************/
#define SERIAL2_TCP_PORT 9200  // Wifi Port UART2

/*********************  Timeout port COM2 *************************/
const long timeoutTime = 100;  // Tempo d'attente reception COM Vetronic en millisecond

/*************  Gestion de la sauvegarde en EEPROM  ****************/
// identifiant pour retrouver les données en EEPROM
#define PARAM_ID "VETRONIC"

// Paramètres de Jeedom
typedef struct
{
  char id[sizeof(PARAM_ID)];
  char  ip_jeedom[16];
  char  api_key_jeedom[33];  
  uint16_t id_code_status;
  uint16_t id_txt_status;
  uint16_t id_tension;
  uint16_t id_courant;
  uint16_t id_courant_max;
  unsigned long freq_update_evse;
} param_t;

/*************  Etat de la borne  ****************/
typedef struct
{
  uint16_t code_status;
  char txt_status[40];
  uint16_t tension;
  uint16_t courant;
  uint16_t courant_max;
} evse_state_t;

/****************  Messages de debug  ******************/
extern bool debug ;

#endif // !_CONFIG_H_


