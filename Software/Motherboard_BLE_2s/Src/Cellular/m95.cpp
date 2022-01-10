/**
 * @file m95.c
 *
 *  Created on: Dec 5, 2018
 *      Author: Joschka Wirges
 */
#include "main.h"
#include "m95.h"
#include "stm32l4xx_hal.h"
#include "usart.h"
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "gpio.h"
#include <string.h>

M95 hM95;

/**
 * @brief acts as a Constructor for the M95 object, powers on the module afterwards
 *  	  the UART-reception can be activated after this function
 * @param *hM95 (M95) pointer to the M95 structure
 */
void M95_Init(M95 *hM95, UART_HandleTypeDef *huart) {
	hM95->huart = huart;
	hM95->rssi = 0;
	hM95->ber = 0;
	hM95->respERR = false;
	hM95->respOK = false;
	hM95->respCME = false;
	hM95->respCMS = false;
	hM95->URC_received = false;
	/* Power switch on */
	HAL_GPIO_DeInit(M95_ON_GPIO_Port, M95_ON_Pin);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = M95_ON_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(M95_ON_GPIO_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BC95_ON_GPIO_Port, BC95_ON_Pin, GPIO_PIN_RESET);
	/*set ANT_M95 = HIGH to switch the SPDT open for the M95 through to the antenna*/
	HAL_GPIO_WritePin(ANT_M95_GPIO_Port, ANT_M95_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(M95_DTR_GPIO_Port, M95_DTR_Pin, GPIO_PIN_RESET);
	/*check if M95 is off*/
	if (HAL_GPIO_ReadPin(M95_STATUS_GPIO_Port, M95_STATUS_Pin)
			== GPIO_PIN_RESET) { //M95 is off if STATUS=0
		HAL_GPIO_WritePin(M95_ON_SW_GPIO_Port, M95_ON_SW_Pin, GPIO_PIN_SET); //Activate M95
		while (HAL_GPIO_ReadPin(M95_STATUS_GPIO_Port, M95_STATUS_Pin)
				== GPIO_PIN_RESET) { //M95 is on, STATUS=1
			printf("- Starting M95 -\r\n");
			HAL_Delay(50);
		}
		printf("- M95 booting -\r\n");
		//Wait_For_URC(&hM95, "RDY", 2000);
		HAL_Delay(6000); //wait for boot to complete
		//TODO replace by Wait_for_URC
		printf("- M95 boot complete -\r\n");
	} else {
		printf("- M95 on before reset -\r\n");
	}
	HAL_GPIO_WritePin(M95_ON_SW_GPIO_Port, M95_ON_SW_Pin, GPIO_PIN_RESET); //Release ON_SW
	/*
	 * M95 uses Autobauding which means it detects the baudrate after booting.
	 * This first command may or may not be interpreted correctly by the module -
	 * it is used to synchronize the UART-Baudrate for further communication
	 */
	bool timed_out = false;
	for (uint8_t i = 0; i < 3; i++)
		timed_out = M95_AT_Command(hM95, "AT", 0, 1000);
	if (timed_out)
		printf("- M95 timed out in M95_Init -\r\n");
	//turn off AT-Command echos since they serve no purpose for our application
	M95_AT_Command(hM95, "ATE0", 0, 1000);
}

/**
 * @brief turns the M95 off and sets the antenna path
 */
void M95_DeInit(void) {
	/* Turn off powerswitch */
	GPIO_InitTypeDef GPIO_InitStruct;
	HAL_GPIO_DeInit(M95_ON_GPIO_Port, M95_ON_Pin);
	GPIO_InitStruct.Pin = M95_ON_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(M95_ON_GPIO_Port, &GPIO_InitStruct);

	/*sets ANT_M95 = LOW to shut off the antenna path from the M95*/
	HAL_GPIO_WritePin(ANT_M95_GPIO_Port, ANT_M95_Pin, GPIO_PIN_RESET);
	/*check if M95 is on*/
	if (HAL_GPIO_ReadPin(M95_STATUS_GPIO_Port, M95_STATUS_Pin)
			== GPIO_PIN_SET) { //M95 is on if STATUS=1
		HAL_GPIO_WritePin(M95_ON_SW_GPIO_Port, M95_ON_SW_Pin, GPIO_PIN_SET); //Turn off M95
		while (HAL_GPIO_ReadPin(M95_STATUS_GPIO_Port, M95_STATUS_Pin)
				== GPIO_PIN_SET) { //M95 is on, STATUS=1
			printf("- Turning off M95 -\r\n");
			HAL_Delay(50);
		}
		printf("- M95 turned off -\r\n");
	} else {
		printf("- M95 off before DeInit -\r\n");
	}
	HAL_GPIO_WritePin(M95_ON_SW_GPIO_Port, M95_ON_SW_Pin, GPIO_PIN_RESET); //Release ON_SW
}

/**
 * @brief decodes lines of AT responses, saves them or raises error-flags for the M95
 * 		  function is called in HAL_UART_RxCpltCallback once a line is complete
 * @param *hM95 (M95) pointer to the M95 structure
 * @param at_response (char[]) one line received by the M95
 */
void M95_Decode_AT_Resp(M95 *hM95, char at_response[]) {
	/*
	 * the first few line characters (c0,c1,c2 etc.) are used a lot in if statements, so the name is
	 * shortened here to increase visibility
	 */
	char c0 = at_response[0], c1 = at_response[1];
	bool URC_flag = false;

	/*because at_response has a size of 100 chars a length check is negligible here*/
	/*check for 'OK' and 'ERROR'*/
	if (c0 == 'O') {
		if (c1 == 'K') {
			hM95->respOK = true;
			//printf("respOK=%d in Decode_AT\r\n", hM95->respOK);
		}
	} else if (c0 == 'E') {
		if (c1 == 'R' && at_response[2] == 'R' && at_response[3] == 'O'
				&& at_response[4] == 'R') {
			hM95->respERR = true;
			printf("received ERROR\r\n");
		}
		/*check for any URC incoming, which could be CME/CMS error codes or
		 * status updates on MQTT/general connection status
		 * '+' is usually the indicator for a general URC, but it might be
		 * a message (this distinction is handled in URC_Handler),
		 * R for RDY-URC, U for undervoltage-URC, C for Call Ready
		 */
	} else if (c0 == '+' || c0 == 'R' || c0 == 'U' || c0 == 'C') {
		URC_flag = M95_Decode_URC(hM95, at_response);
		if (!URC_flag) { //a message that was assumed to be a URC was a message
			if (hM95->lines_received == 0) {
				head = prepend(head, at_response);
				hM95->lines_received++;
			} else {
				head = append(head, at_response);
				hM95->lines_received++;
			}
		}
	} else { //normal response line incoming
		if (hM95->lines_received == 0) {
			head = prepend(head, at_response);
			hM95->lines_received++;
		} else {
			head = append(head, at_response);
			hM95->lines_received++;
		}
	}
}

/**
 * @brief checks for a final response sent by the M95
 * @retvalue true if final_response received, else false
 */
bool M95_Final_Response(M95 *hM95) {
	if ((hM95->respERR) || (hM95->respOK) || (hM95->respCME)
			|| (hM95->respCMS)) {
		return true;
	} else {
		return false;
	}
}

bool M95_Wait_For_URC(M95 *hM95, char urc[], uint32_t timeout) {
	uint32_t tickstart = HAL_GetTick();
	bool timed_out = false;
	while (!timed_out) {
		/* Check for the timeout */
		if (timeout != HAL_MAX_DELAY) {
			if ((timeout == 0U) || ((HAL_GetTick() - tickstart) > timeout)) {
				timed_out = true;
			}
		}
		asm("nop");
	}
	return timed_out;
}

/**
 * @brief executes an AT-Command with a  timeout
 * @param *hM95 (M95) pointer to the M95 structure
 * @param cmd[] (char) at-command without CR ('\r')
 * @param timeout (uint32_t) timeout for the AT-Command execution until final response
 * 			  or CMS/CME Error
 * @retval timed_out (bool) true if timeout occured, false if AT-command succesfully executed
 */
bool M95_AT_Command(M95 *hM95, char cmd[], uint8_t lines_to_receive,
		uint32_t timeout) {
	hM95->respCME = false;
	hM95->respCMS = false;
	hM95->respERR = false;
	hM95->respOK = false;
	hM95->lines_received = 0;
	uint32_t tickstart = HAL_GetTick(); //current timer as a reference for the timeout
	bool timed_out = false; //return value
	bool receiving = true;

	/* construct the command */
	uint8_t len = strlen(cmd); //the full command needs a CR ('\r') to be accepted by the module
	char full_cmd[len + 1];
	for (uint8_t i = 0; i < len; i++) {
		full_cmd[i] = cmd[i];
	}
	full_cmd[len] = '\r';

	/* transmit the command' */
	HAL_UART_Transmit(hM95->huart, (uint8_t *) full_cmd, (len + 1), 1000);
//	printf("!M95_Final_Response(hM95)=%d && (!timed_out)=%d ==%d\r\n",!M95_Final_Response(hM95),(!timed_out)
//				,!(M95_Final_Response(hM95)) && (!timed_out));
	/* Wait for timeout or final response (ERROR/OK/CME- or CMS-Error) */

	while (receiving && (!timed_out)) {

		/* Check for the timeout */
		if (timeout != HAL_MAX_DELAY) {
			if ((timeout == 0U) || ((HAL_GetTick() - tickstart) > timeout)) {
				timed_out = true;
				printf("- %s timed out -\r\n", cmd);
			}
		}
		/* if a final response like OK, ERROR etc. is received and all
		 * lines_to_receive are received, then set receiving=false to stop the while()
		 */
		//printf("M95_Final_Response(hM95)=%d\r\n",M95_Final_Response(hM95));
		if (M95_Final_Response(hM95)
				&& (hM95->lines_received) >= lines_to_receive) {
			receiving = false;
		}
		/* Assembler instruction "no operation" used here to make the
		 execution wait, suppressing unnecessary timeout-checks for the same tick*/
		asm("nop");
		//HAL_Delay(10);
	}
	if (!timed_out && hM95->URC_received) {
		printf("A URC was received during AT execution\r\n");
	}
	return timed_out;
}

/**
 * @brief looks for specific string sequences to determine what kind of message was received
 * 		  possibilites are status updates, error messages or normal response lines where the latter case
 * 		  has the lowest probability to occur, most messages starting with a '+'-char are status updates
 * @param *hM95 (M95) pointer to the M95 structure
 * @param at_response (char[]) one line received by the M95 (that is assumedly a URC)
 * @retvalue signals if a URC (true) or a message (false) was received
 */

bool M95_Decode_URC(M95 *hM95, char at_response[]) {
	/*
	 * the first few line characters are used a lot in if statements, so the name is
	 * shortened here to increase visibility
	 */
	char c0 = at_response[0], c1 = at_response[1], c2 = at_response[2], c3 =
			at_response[3], c4 = at_response[4];
	/*
	 * starting with character + the following list of URCs/Messages is looked for:
	 * +CREG,+CME Error,+CMS Error
	 */
	if (c0 == '+') { //looking for general URCs like QMSTAT, Message response line like +CREG and errors +CME/+CMS
		if (c1 == 'C') {
			if (c2 == 'R' && c3 == 'E' && c4 == 'G') { //CREG
				return false; //CREG is not a URC but a response to AT+CREG?
			} else if (c2 == 'M') { //CME/CMS-Error
				if (c3 == 'E') { //CME-Error
					hM95->respCME = true;
					memset(hM95->cme_cms_error, 0, 4);
					hM95->cme_cms_error[0] = at_response[12];
					hM95->cme_cms_error[1] = at_response[13];
					hM95->cme_cms_error[2] = at_response[14];
					hM95->cme_cms_error[3] = at_response[15];
					printf("received CME Error with ID %c%c%c%c\r\n",
							hM95->cme_cms_error[0], hM95->cme_cms_error[1],
							hM95->cme_cms_error[2], hM95->cme_cms_error[3]);
				} else if (c3 == 'S') { //CMS-Error
					hM95->respCMS = true;
					memset(hM95->cme_cms_error, 0, 4);
					hM95->cme_cms_error[0] = at_response[12];
					hM95->cme_cms_error[1] = at_response[13];
					hM95->cme_cms_error[2] = at_response[14];
					hM95->cme_cms_error[3] = at_response[15];
					printf("received CMS Error with ID %c%c%c%c\r\n",
							hM95->cme_cms_error[0], hM95->cme_cms_error[1],
							hM95->cme_cms_error[2], hM95->cme_cms_error[3]);

				}
			}

		} else if (c0 == 'R') { //looking for RDY
			if (c1 == 'D' && at_response[2] == 'Y') {
				hM95->URC_received = true;
			}
		} else if (c0 == 'U') { //looking for UNDERVOLTAGE_WARNING
		} else if (c0 == 'C') { //looking for Call Ready
		}

	}

	hM95->URC_received = false;
	//TODO
	//CME
	//CMS
	//+QMT, +COPS etc.
	//UNDER_VOLTAGE_WARNING
	return false;
}
/**
 * @brief checks for a detected SIM by retrieving the IMSI, configures APN, checks
 * 		  for established connection
 * @param *hM95 (M95) pointer to the M95 structure
 * @retvalue @retvalue signals if a connection has been established (true) or not (false)
 */
bool M95_Connect(M95 *hM95) {
	if (!M95_AT_Command(hM95, "AT+CIMI", 1, 1000)) {
		strcpy(hM95->imsi, head->line);
	}
	printf("SIM found, IMSI: %s", hM95->imsi);
	/* check for connection */
	bool connected = false;
	/*while (!_connected) {
	 if (!M95_AT_Command(&hM95, "AT+CREG?", 1, 1000)) {
	 if (head->line[9] == '5') {
	 connected = true;
	 }
	 head = dispose(head);
	 }
	 HAL_Delay(2500);
	 }*/
	connected = false;
	while (!connected) {
		if (!M95_AT_Command(hM95, "AT+CGATT?", 1, 1000)) {
			if (head->line[8] == '1') {
				connected = true;
			}
		}
		HAL_Delay(2500);
	}

	if (!M95_AT_Command(hM95, "AT+COPS?", 1, 1000)) {
		for (uint8_t i = 0; i < ((head->length) - 15); i++) {
			hM95->mobile_operator[i] = head->line[i + 12];
		}
		printf("- Connected to %s -\r\n", hM95->mobile_operator);
	}

	printf("- Configuring APN as iot.1nce.net -\r\n");
	M95_AT_Command(hM95, "AT+QICSGP=1,\"iot.1nce.net\"", 0, 1000);
	HAL_Delay(2000);
	return connected;
}

bool M95_MQTTPub(M95 *hM95, char message[]) {
	bool message_published = false;
	printf("- Connecting to test.mosquitto.org:1883 -\r\n");
	message_published = M95_AT_Command(hM95,
			"AT+QMTOPEN=0,\"10.64.72.81\",1883", 0, 1000);
	HAL_Delay(2000); //Wait for URC +QMTOPEN: 0,0

	message_published = M95_AT_Command(hM95, "AT+QMTCONN=0,\"M95_7\"", 0, 1000);
	HAL_Delay(2000);
	printf("- Publishing in Topic rhizotech/sensorbox1/values -\r\n");
	M95_AT_Command(hM95, "AT+QMTPUB=0,0,0,0,\"rhizotech/sensorbox1/values\"", 0,
			1000);

	printf("- Disconnecting from the MQTT Broker -\r\n");
	HAL_UART_Transmit(hM95->huart, (uint8_t *) message, strlen(message), 1000);
	//M95_AT_Command(&hM95, "AT+QMTDISC=0", 0, 1000);
	//M95_AT_Command(&hM95, "AT+QMTCLOSE=0", 0, 1000);
	message_published = true;
	//TODO message_published = wait_for_urc +QMTPUB 0,0,0
	return message_published;
}
