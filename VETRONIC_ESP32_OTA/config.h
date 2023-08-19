#include <String.h>

/*************************  Constante ****************************/
#define VERSION "1.1"
#define ATTENTE_REQUETE 1
#define READ_EVSE 2
#define ENVOI_REQ 3
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

/****************  Active les messages de debug  ******************/
bool debug = false;




