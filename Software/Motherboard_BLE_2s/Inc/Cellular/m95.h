/**
 * @file m95.h
 *
 *  Created on: Nov 29, 2018
 *      Author: Joschka Wirges
 */

/**
 *  @brief stores information on the state of the 2G-module Quectel M95
 *
 * @param huart (UART_HandleTypeDef *) pointer to the M95 UART interface
 * @param rssi (uint8_t) stores the current RSSI with 0-31 and 99 as possible values
 * 		  0=-113dBm or less,1=-111dBm, 2...30=-109...-53dBm and 99=not known/undetectable
 * 		  issued with AT+CSQ(=rssi,ber)
 * @param ber (uint8_t) stores the current BER with 0-7 and 99 as possible values
 * 		  0...7=RXQUAL_0...RXQUAL_7 of GSM 05.08 subclause 8.2.4 (assumed values in % below) and 99=not known/undetectable
 * 		  Assumed BER in % for the RXQUAL values from GSM 05.08 subclause 8.2.4 by ETSI
 * 		  RXQUAL_0=0.14%
 * 		  RXQUAL_1=0.28% (RXQUAL_0*2)
 * 		  RXQUAL_2=0.57% (RXQUAL_0*2*2)
 * 	      RXQUAL_3=1.13% (...)
 * 		  RXQUAL_4=2.26%
 * 		  RXQUAL_5=4.53%
 * 		  RXQUAL_6=9.05%
 * 		  RXQUAL_7=18.10%
 * 		  issued with AT+CSQ(=rssi,ber)
 * @param cme_cms_error (char[]) stores any latest CME/CMS-error the device has issued as a response to an AT-command
 *        a list of all CME/CMS errors can be found in the appendix of the AT-Commands Manual
 * @param imsi (char[]) IMSI of the SIM connected to the M95 issued with AT+CIMI
 * @param firmware_ver (char[]) stores the firmware version of the M95 which implies,
 * 		  if certain operations are possible to execute (MQTT,TLS) issued with AT+QGMR?
 * @param operator (char[]) stores the operator of the attached cellular network
 * 		  issued with AT+COPS?
 * @param URC (char[]) stores the last received URC
 * @param respERR (bool) ERROR received from the module (final response)
 * @param respOK (bool) OK received from the module (final response)
 * @param respCME (bool) CME error code received from the module (final response)
 * @param respCMS (bool) CMS error code from the module (final response)
 * @param URC_received (bool) flag to signal a received URC
 *
 */
#include "stdbool.h"

typedef struct M95 {
	UART_HandleTypeDef *huart;
	uint8_t rssi, ber;
	uint8_t lines_received;
	char cme_cms_error[4];
	char imsi[33];
	char firmware_ver[20];
	char mobile_operator[30];
	char URC[100];
	bool respERR;
	bool respOK;
	bool respCME;
	bool respCMS;
	bool URC_received;
}M95;

/* M95 function prototypes */
void M95_Init(M95 *hM95, UART_HandleTypeDef *huart);
void M95_DeInit(void);
void M95_Decode_AT_Resp(M95 *hM95, char at_response[]);
bool M95_Final_Response(M95 *hM95);
bool M95_Wait_For_URC(M95 *hM95, char urc[], uint32_t timeout);
bool M95_AT_Command(M95 *hM95, char cmd[],uint8_t lines_to_receive, uint32_t timeout);
bool M95_Decode_URC(M95 *hM95,char at_response[]);
bool M95_Connect(M95 *hM95);
bool M95_MQTTPub(M95 *hM95, char message[]);

extern M95 hM95;
