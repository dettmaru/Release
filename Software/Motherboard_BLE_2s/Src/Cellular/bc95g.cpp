/**
 * @file bc95g.c
 *
 *  Created on: Dec 5, 2018
 *      Author: Joschka Wirges, Daniel Lohmann
 */
#include "main.h"
#include "linkedlist.h"
#include "bc95g.h"
#include "stm32l4xx_hal.h"
#include "usart.h"
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include "mqttsn.h"
#include <string.h>
#include <stdlib.h>
#include "Energy.h"
#include "StringHelper.h"
#include "Logging.h"
#include "Configuration.h"
#include "queue"

const char* NBIoTTag = "NB-IoT";
extern std::queue<char*>* bc95gQueuePtr;
BC95G hBC95G;

/**
 * @brief Constructor for the BC95G object, powers on the module afterwards
 *  	  the UART-reception can be activated after this function, turns the LTC's high power mode on
 * @return true, when the initialization was successfull and false in case off an error
 */
bool BC95G::Init(UART_HandleTypeDef *huart) {
	this->huart = huart;
	URC[0] = '\0';
	rssi[0] = '\0';
	lines_received = 0;
	cme_cms_error[0] = '\0';
	imsi[0] = '\0';
	firmware_ver[0] = '\0';
	mobile_operator[0] = '\0';
	URC[0] = '\0';
	//operator_mode[0] = '\0';
	//band[0] = '\0';
	respERR = false;
	respOK = false;
	respCME = false;
	respCMS = false;
	URC_received = false;
	registered = false;
	attached = false;
	socketOpen = false;
	udp_ack = false;
	udp_nack = false;
	PSM_active = false;
	MQTT_error = false;
	MQTT_connected = false;
	MQTT_published = false;

	char buffer[100];

	/* Power switch on */
	HAL_GPIO_WritePin(BC95_ON_GPIO_Port, BC95_ON_Pin, GPIO_PIN_RESET);
	/*set ANT_BC95 = HIGH to switch the SPDT open for the BC95 through to the antenna*/
	HAL_GPIO_WritePin(ANT_BC95_GPIO_Port, ANT_BC95_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BC95_RST_GPIO_Port, BC95_RST_Pin, GPIO_PIN_RESET);
	LOGV(NBIoTTag, "BC95G booting ...");

	//Wait for "OK" which signals completed boot process
	if (Wait_For_Final_Response(10000)){
		LOGE(NBIoTTag, "BC95G booting timed out");
		return false;
	}
	HAL_Delay(3000); //Grace period before the first AT Command
	LOGV(NBIoTTag, "BC95G boot complete");
//	AT_Command("AT+QGMR", 1, 1000);
//	strcpy(firmware_ver, head->line);
//	//head = dispose(head);
//	LOGV(NBIoTTag, "FW Version: %s", firmware_ver);

	//turn off AT-command echos ATE0, turn on =ATE1
	if (AT_Command("ATE0", 0, 1000)){ 
		return false;
	}
//	if (!AT_Command("AT+CIMI", 1, 1000)) { //retrieve IMSI
//		strcpy(imsi, head->line);
//		head = dispose(head);
//		LOGV(NBIoTTag, "IMSI: %s", imsi);
//	}
	if (AT_Command("AT+CMEE=1", 0, 300)){
		return false;
	}

	// maximum response time 85s, limited to 10s
	if (AT_Command("AT+CFUN=1", 0, 10000)){ // Turn on the module (UE) full functionality 
		return false;
	}
	if (AT_Command("AT+CPSMS=1,,,00000011,01000001", 0, 300)){
		return false;
	}
	if (AT_Command("AT+NCONFIG=CR_0354_0338_SCRAMBLING,TRUE", 0, 300)){
		return false;
	}
	if (AT_Command("AT+NCONFIG=CR_0859_SI_AVOID,TRUE", 0, 300)){
		return false;
	}
	if (AT_Command("AT+CSCON=1", 0, 300)){ //enable Signalling connection status URCs
		return false;
	} 
	if (AT_Command("AT+NPSMR=1", 0, 300)){ //enable PSM URCs
		return false;
	}
	if (AT_Command("AT+QREGSWT=2", 0, 300)){ //disable automatic registration to Huawei's IoT platform
		return false;
	}

	/* set name of the APN - Access Point Name */
	uint8_t* apn = Configuration::GetString(StringOption::NBIoT_APN, nullptr);
	snprintf(buffer, sizeof(buffer), "AT+CGDCONT=1,\"IP\",\"%s\"", (char*)apn);
	free(apn);
	if (AT_Command((char*)buffer, 0, 300)){
		return false;
	}
	if (AT_Command("AT+CEREG=5", 0, 300)){ //enable registration status URCs
		return false;
	}

	/*
	 Network location area information
	 MCC (three digits) + MNC (two or three digits)
	 MCC - Mobile country code
	 MNC - Mobile network code
	 e.g. List of codes: https://www.mcc-mnc.com/
	 */
	uint8_t* network = Configuration::GetString(StringOption::NBIoT_Network, nullptr);
	snprintf(buffer, sizeof(buffer), "AT+COPS=1,2,\"%s\"", (char*)network);
	free(network);
	// maximum response time 630s, limited to 1s
	if (AT_Command((char*)buffer, 0, 1000)){ //set APN to e.g. 26201 (Telekom DE)
		return false;
	}

	return true;
}

/**
 * @brief Reinitialized the BC95G struct after a standby mode
 * @return true, when the initialization was successfull and false in case off an error
 */
bool BC95G::ReInit(UART_HandleTypeDef* huart)
{
	this->huart = huart;
	URC[0] = '\0';
	rssi[0] = '\0';
	lines_received = 0;
	cme_cms_error[0] = '\0';
	imsi[0] = '\0';
	firmware_ver[0] = '\0';
	mobile_operator[0] = '\0';
	URC[0] = '\0';
	//operator_mode[0] = '\0';
	//band[0] = '\0';
	respERR = false;
	respOK = false;
	respCME = false;
	respCMS = false;
	URC_received = false;
	registered = false;
	attached = false;
	socketOpen = false;
	udp_ack = false;
	udp_nack = false;
	PSM_active = false;
	MQTT_error = false;
	MQTT_connected = false;
	MQTT_published = false;

	LOGV(NBIoTTag, "Reinit BC95G");

	AT_Command("AT+CEREG?", 0, 350);
	HAL_Delay(1000);
	if (!AT_Command("AT+CGATT?", 1, 1000)) {
		if (head->line[7] == '1') {
			attached = true;
			LOGV(NBIoTTag, "attached");
		}
		LOGV(NBIoTTag, "%s", head->line);
	}
	else
	{
		LOGE(NBIoTTag, "attached request failed");
	}

	return true;
}

/**
 * @brief turns the BC95G off and sets the antenna path, returns the LTC to low power mode
 */
void BC95G::DeInit(void) {
	LOGV(NBIoTTag, "Powering off BC95G");
	/*sets ANT_M95 = LOW to shut off the antenna path from the BC95G*/
	HAL_GPIO_WritePin(ANT_BC95_GPIO_Port, ANT_BC95_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BC95_RST_GPIO_Port, BC95_RST_Pin, GPIO_PIN_SET); //Turn off BC95G
	HAL_Delay(500);
	/* Power switch off */
	HAL_GPIO_WritePin(BC95_ON_GPIO_Port, BC95_ON_Pin, GPIO_PIN_SET);
}

/**
 * @brief Resets the BC95G
 */
void BC95G::Reset(void) {
	HAL_GPIO_WritePin(BC95_RST_GPIO_Port, BC95_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(BC95_ON_GPIO_Port, BC95_ON_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
}

/**
 *  @brief executes an AT-Command with a  timeout
 * @param cmd[] (char) at-command without CR ('\r')
 * @param timeout (uint32_t) timeout for the AT-Command execution until final response
 * 			  or CMS/CME Error
 * @retval timed_out (bool) true if timeout occurred or and other error occurred, 
 * 							false if AT-command successfully executed
 */
bool BC95G::AT_Command(char cmd[], uint8_t lines_to_receive,
		uint32_t timeout) {
	respCME = false;
	respCMS = false;
	respERR = false;
	respOK = false;
	lines_received = 0;
	uint32_t tickstart = HAL_GetTick(); //current timer as a reference for the timeout
	bool timed_out = false; //return value
	bool receiving = true;

	/* construct the command */
	uint32_t len = strlen(cmd); //the full command needs a CR ('\r') to be accepted by the module
	char full_cmd[len + 1];
	for (uint32_t i = 0; i < len; i++) {
		full_cmd[i] = cmd[i];
	}
	full_cmd[len] = '\r';

	/* transmit the command' */
	HAL_UART_Transmit(huart, (uint8_t *) full_cmd, (len + 1), 1000);
	/* Wait for timeout or final response (ERROR/OK/CME- or CMS-Error) */
	while (receiving && (!timed_out)) {

		ProcessMessageQueue();

		/* Check for the timeout */
		if (timeout != HAL_MAX_DELAY) {
			if ((timeout == 0U) || ((HAL_GetTick() - tickstart) > timeout)) {
				timed_out = true;
				LOGV(NBIoTTag, "- %s timed out -", cmd);
			}
		}
		/* if a final response like OK, ERROR etc. is received and all
		 * lines_to_receive are received, then set receiving=false to stop the while()
		 */
		if (Final_Response()
				&& (lines_received) >= lines_to_receive) {
			if (respERR)
				LOGV(NBIoTTag, "%s responded with ERROR", cmd);
			receiving = false;
		}
		/* Assembler instruction "no operation" used here to make the
		 execution wait, suppressing unnecessary timeout-checks for the same tick*/
		asm("nop");
	}
	if (!timed_out && URC_received) {
		LOGV(NBIoTTag, "A URC was received during AT execution");
	}
	return timed_out || respCME || respCMS || respERR;
}

/**
 * @brief checks for a final response to an AT-Command (OK, ERROR, CME/CMS-Error) has been sent by the BC95G
 * 		  this functions only use is the abbreviation of a relatively long boolean operator
 * @retvalue true if final response received, else false
 */
bool BC95G::Final_Response() {
	if ((respERR) || (respOK) || (respCME) || (respCMS)) {
		return true;
	} else {
		return false;
	}
}

/**
 * @brief checks for a final response sent by the BC95G
 * @param timeout (uint32_t) time to wait for the URC until timeout
 * @retvalue true if timed out while waiting, false if final response was received
 */
bool BC95G::Wait_For_Final_Response(uint32_t timeout) {
	bool timed_out = false;
	uint32_t tickstart = HAL_GetTick(); //current timer as a reference for the timeout
	while (!Final_Response() && (!timed_out)) {
		ProcessMessageQueue();
		/* Check for the timeout */
		if (timeout != HAL_MAX_DELAY) {
			if ((timeout == 0U) || ((HAL_GetTick() - tickstart) > timeout)) {
				timed_out = true;
				LOGV(NBIoTTag, "- timed out while waiting for a final response -");
			}
		}
		/* Assembler instruction "no operation" used here to make the
		 execution wait, suppressing unnecessary timeout-checks for the same tick*/
		asm("nop");
	}
	return timed_out;
}

/**
 * @brief Process message queue
 */
void BC95G::ProcessMessageQueue() {
	__disable_irq();

	while (!bc95gQueuePtr->empty())
	{
		char* message = bc95gQueuePtr->front();
		bc95gQueuePtr->pop();
		__enable_irq();

		Decode_AT_Resp(message);

		free(message);
	}

	__enable_irq();

	HAL_Delay(5);

	// Attention: Independent watch dog (IWDG) Reset!
	HAL_IWDG_Refresh(&hiwdg);
}

/**
 * @brief decodes lines of AT responses, saves them or raises error-flags for the BC95G
 * 		  function is called in HAL_UART_RxCpltCallback once a line is complete
 * @param at_response (char[]) one line received by the BC95G
 */
void BC95G::Decode_AT_Resp(char at_response[]) {
	/*
	 * the first few line characters are used a lot in if statements, so the name is
	 * shortened here to increase visibility
	 */
	char c0 = at_response[0], c1 = at_response[1], c2 = at_response[2], c3 =
			at_response[3], c4 = at_response[4];
	bool URC_flag = false;

	/*because at_response has a size of 100 chars a length check is negligible here*/
	/*check for 'OK' and 'ERROR'*/
	if (c0 == 'O') {
		if (c1 == 'K') {
			respOK = true;
		}
	} else if (c0 == 'E') {
		if (c1 == 'R' && c2 == 'R' && c3 == 'O' && c4 == 'R') {
			respERR = true;
		}
		/*check for any URC incoming, which could be CME/CMS error codes or
		 * status updates on MQTT/general connection status
		 * '+' is usually the indicator for a general URC, but it might be
		 * a message (this distinction is handled in URC_Handler)
		 */
	} else if (c0 == '+') {
		URC_flag = Decode_URC(at_response);
		if (!URC_flag) { //a message that was assumed to be a URC was a message
			if (lines_received == 0) {
				head = prepend(head, at_response);
				lines_received++;
			} else {
				head = append(head, at_response);
				lines_received++;
			}
		}
	} else if ((c0 == 'P' && c1 == 'r') || (c0 == 'S' && c1 == 'e')
			|| (c0 == 'A' && c1 == 'p') || (c0 == 'B' && c1 == 'o')
			|| (c0 == 'R' && c1 == 'E') || (c0 == 'N' && c1 == 'e')) { //filter out bootloader messages
		//LOGV(NBIoTTag, "filtered bootloader message: %s", at_response);
		return;
	} else { //normal response line incoming
		if (lines_received == 0) {
			head = prepend(head, at_response); //head = prepend(NULL, at_response);
			lines_received++;
		} else {
			head = append(head, at_response);
			lines_received++;
		}
	}
}

/**
 * @brief checks for a detected SIM by retrieving the IMSI, configures APN, checks
 * 		  for established connection (connection is established automatically once
 * 		  the module is configured)
 * @retvalue signals if a connection has been established (true) or not (false)
 */
bool BC95G::Connect() {
	uint16_t attempts = 0;
	registered = false;
	attached = false;
	/* check for connection */
	while (!registered && attempts < 600) { // 600attempts*100ms delay=60s timeout
		ProcessMessageQueue();
		HAL_Delay(100);
		attempts++;
	}
	if (registered) {
		LOGV(NBIoTTag, "- BC95G registered to operator -");
	} else {
		LOGV(NBIoTTag, 
				"- BC95G not registered to network after 60 seconds of waiting -");
		return false;
	}
	attempts = 0;
	while (!attached && attempts < 600) { // 600attempts*100ms delay=60s timeout
		if (!AT_Command("AT+CGATT?", 1, 1000)) {
			if (head->line[7] == '1') {
				attached = true;
			}
		}
		HAL_Delay(100);
		attempts++;
	}
	if (attached) {
		LOGV(NBIoTTag, "- BC95G attached to cell -");
	} else {
		LOGV(NBIoTTag, 
				"- BC95G not attached to cell after 60 seconds of waiting -");
		return false;
	}

	LOGV(NBIoTTag, "- Attached to network -");
	if (!AT_Command("AT+COPS?", 1, 1000)) {
		for (uint8_t i = 0; i < ((head->length) - 14); i++) {
			mobile_operator[i] = head->line[i + 11];
		}
		LOGV(NBIoTTag, "- Connected to %s -", mobile_operator);
	}
	else
	{
		return false;
	}
//	AT_Command("AT+CSQ", 1, 1000);
//	Extract_Information_CSQ(head->line);
//	AT_Command("AT+NUESTATS=RADIO", 13, 1000);
//	Extract_Information_NUESTATS();
	return true;
}

/**
 * @brief looks for specific string sequences to determine what kind of message was received
 * 		  possibilities are status updates, error messages or normal response lines where the latter case
 * 		  has the lowest probability to occur, most messages starting with a '+'-char are status updates
 * 		  the extraction of information is done in separate functions per message to be decoded
 * 		  bool BC95G_Extract_Information_<URC>(BC95G *hBC95G, char at_response[])
 * @param at_response (char[]) one line received by the BC95G (that is assumedly a URC)
 * @retvalue signals if a URC (true) or a message (false) was received
 */

bool BC95G::Decode_URC(char at_response[]) {
	/*
	 * the first few characters of at_response are used often. these have own
	 * variables c0, c1, ... (char0, char1, ...) to increase readability
	 */
	char c0 = at_response[0], c1 = at_response[1], c2 = at_response[2], c3 =
			at_response[3], c4 = at_response[4], c5 = at_response[5];
	/*
	 * starting with character + the following list of URCs/Messages is looked for:
	 * +CEREG,+CME Error,+CMS Error
	 */
	if (c0 == '+') { //looking for general URCs like QMSTAT, Message response line like +CEREG and errors +CME/+CMS
		if (c1 == 'C') {
			if (c2 == 'E' && c3 == 'R' && c4 == 'E' && c5 == 'G') { //CEREG
				return Extract_Information_CEREG(at_response);
			} else if (c2 == 'M') { //CME/CMS-Error
				if (c3 == 'E') { //CME-Error
					return Extract_Information_CME(at_response);
				} else if (c3 == 'S') { //CMS-Error
					return Extract_Information_CMS(at_response);
				}
			}
		}
		if (c1 == 'N' && c2 == 'S' && c3 == 'O' && c4 == 'S' && c5 == 'T') {
			return Extract_Information_NSOSTR(at_response);
		}
		if (c1 == 'N' && c2 == 'P' && c3 == 'S' && c4 == 'M' && c5 == 'R') {
			return Extract_Information_NPSMR(at_response);
		}
		if (c1 == 'N' && c2 == 'Q' && c3 == 'S' && c4 == 'O' && c5 == 'S') {
			return Extract_Information_NQSOS(at_response);
		}
		if (c1 == 'C' && c2 == 'S' && c3 == 'C' && c4 == 'O' && c5 == 'N') {
			return Extract_Information_CSCON(at_response);
		}
		if (c1 == 'Q' && c2 == 'M' && c3 == 'T') { //QMTPUB, QMTOPEN, QMTCONN
			return Extract_Information_QMT(at_response);
		}
		if (c1 == 'C' && c2 == 'S' && c3 == 'Q') {
			//return Extract_Information_CSQ(at_response);
		}
		if (c1 == 'C' && c2 == 'P' && c3 == 'S' && c4 == 'M' && c5 == 'S') {
			return Extract_Information_CPSMS(at_response);
		}
	}
	/* no if-case was true, there was no URC */
	URC_received = false;
	return false;
}

/**
 * @brief extracts information from URC +CEREG:
 * @param at_response (char[]) one line received by the BC95G (that is assumedly a URC)
 * @retvalue signals if information could be extracted (true) or not (false)
 */
bool BC95G::Extract_Information_CEREG(char at_response[]) {
	bool ret = true;
	// Differentiate between URC and normal response, this is done here based on 
	// the number of commas return, because a reliable detection of the URC is 
	// not possible
	int count = strcountchar(at_response, ','); 
	char stat = at_response[count == 4 ? 9 : 7];
	//LOGV(NBIoTTag, "%s", at_response);
	if (stat == '0') {
		LOGV(NBIoTTag, "- CEREG:0 not registered, looking for operator -");
		registered = false;
		ret = true;
	} else if (stat == '1') {
		LOGV(NBIoTTag, "- CEREG:1 registered, home network -");
		registered = true;
		ret = true;
	} else if (stat == '2') {
		LOGV(NBIoTTag, "- CEREG:2 not registered, looking for operator -");
		registered = false;
		ret = true;
	} else if (stat == '3') {
		LOGV(NBIoTTag, "- CEREG:3 registration denied by operator -");
		registered = false;
		ret = true;
	} else if (stat == '4') {
		LOGV(NBIoTTag, 
				"- CEREG:4 registration state unknown, possibly out of coverage -");
		registered = false;
		ret = true;
	} else if (stat == '5') {
		LOGV(NBIoTTag, "- CEREG:5 registered, roaming -");
		registered = true;
		ret = true;
	}
	return ret;
}

/**
 * @brief extracts information from Error URC +CME:
 * @param at_response (char[]) one line received by the BC95G (that is assumedly a URC)
 * @retvalue signals if information could be extracted (true) or not (false)
 */
bool BC95G::Extract_Information_CME(char at_response[]) {
	bool ret = false; //return false in case of Error URC, flag notifies of an error occurrence
	respCME = true;
	memset(cme_cms_error, 0, 4);
	cme_cms_error[0] = at_response[12];
	cme_cms_error[1] = at_response[13];
	cme_cms_error[2] = at_response[14];
	cme_cms_error[3] = at_response[15];
	LOGE(NBIoTTag, "received CME Error with ID %c%c%c%c",
			cme_cms_error[0],
			cme_cms_error[1], 
			cme_cms_error[2],
			cme_cms_error[3]);

	return ret;
}

/**
 * @brief extracts information from Error URC +CMS:
 * @param at_response (char[]) one line received by the BC95G (that is assumedly a URC)
 * @retvalue signals if information could be extracted (true) or not (false)
 */
bool BC95G::Extract_Information_CMS(char at_response[]) {
	bool ret = false; //return false in case of Error URC, flag notifies of an error occurrence
	respCMS = true;
	memset(cme_cms_error, 0, 4);
	cme_cms_error[0] = at_response[12];
	cme_cms_error[1] = at_response[13];
	cme_cms_error[2] = at_response[14];
	cme_cms_error[3] = at_response[15];
	LOGE(NBIoTTag, "received CMS Error with ID %c%c%c%c",
			cme_cms_error[0],
			cme_cms_error[1], 
			cme_cms_error[2],
			cme_cms_error[3]);

	return ret;
}

/**
 * @brief extracts information from URC +NPSMR:
 * @param at_response (char[]) one line received by the BC95G (that is assumedly a URC)
 * @retvalue signals if information could be extracted (true) or not (false)
 */
bool BC95G::Extract_Information_NPSMR(char at_response[]) {
	bool ret = true; //NPSMR is a URC, no further checks needed
	char npsmr = at_response[7];
	if (npsmr == '1') {
		PSM_active = true;
		LOGV(NBIoTTag, "- BC95G entered power saving mode (PSM) -");
	} else {
		PSM_active = false;
		LOGV(NBIoTTag, "- BC95G entered normal power mode -");
	}
	return ret;
}

/** 
 * @brief extract information from message +CPSMS:
 * @param at_response (char[]) one line received by the BC95G
 * @retvalue signals if information was a URC (true) or not (false)
*/
bool BC95G::Extract_Information_CPSMS(char at_response[]) {
	bool ret = false;
	//+CPSMS:1,,,01000001,00000101
	LOGI(NBIoTTag, "%s", at_response);
	char cpsms = at_response[7];
	if (cpsms == '1') {
		char periodicTAU[9] = {0};
		char activeTime[9] = {0};
		memcpy(periodicTAU, at_response + 11, 8);
		memcpy(activeTime, at_response + 20, 8);
		periodic_TAU = strtol(periodicTAU, NULL, 2);
		active_time = strtol(activeTime, NULL, 2);
		time_t act = GetActiveTime();
		LOGI(NBIoTTag, "Active Time: %d", (int)act);
		time_t tau = GetPeriodicTAU();
		LOGI(NBIoTTag, "Periodic Tau: %d", (int)tau);
	} else {
		PSM_active = false;
	}
	return ret;
}

/**
 * @brief extracts information from URC +CSCON:
 * @param at_response (char[]) one line received by the BC95G (that is assumedly a URC)
 * @retvalue signals if information could be extracted (true) or not (false)
 */
bool BC95G::Extract_Information_CSCON(char at_response[]) {
	bool ret = true; //CSCON is a URC, no further checks needed
	char cscon = at_response[7];
	if (cscon == '1') {
		LOGV(NBIoTTag, "- BC95G signaling connection status active  -");
	} else {
		LOGV(NBIoTTag, "- BC95G signaling connection status idle -");
	}
	return ret;
}

/**
 * @brief * extracts information from MQTT URCs starting with +QMT( +QMTPUB, +QMTCONN, +QMTOPEN):
 * @param at_response (char[]) one line received by the BC95G (that is assumedly a URC)
 * @retvalue signals if information could be extracted (true) or not (false)
 */
bool BC95G::Extract_Information_QMT(char at_response[]) {
	bool ret = true; //QMT<> are all URCs, no further checks needed
	MQTT_connected = false; //The BC95G as a MQTT-Client is not connected unless a URC stating otherwise says so
	volatile char c4 = at_response[4], c5 = at_response[5], c6 = at_response[6];
	//+QMTPUB: <tcpconnectID>,<msgID>,<result> only result is relevant
	if (c4 == 'P' && c5 == 'U' && c6 == 'B') {

		if (at_response[13] == '0') {
			MQTT_error = false;
			MQTT_published = true;
			LOGV(NBIoTTag, "- BC95G MQTT packet successfully sent -");
		} else if (at_response[13] == '1') {
			MQTT_error = true;
			LOGV(NBIoTTag, "- BC95G MQTT packet retransmission -");
		} else if (at_response[13] == '2') {
			MQTT_error = true;
			LOGV(NBIoTTag, "- BC95G failed to send MQTT packet -");
		}

		//+QMTCONN: <tcpconnectID>,<result>,[<ret_code>] only result is relevant
	} else if (c4 == 'C' && c5 == 'O' && c6 == 'N') {
		MQTT_error = true;
		if (at_response[12] == '0') {
			MQTT_error = false;
			MQTT_connected = true;
			LOGV(NBIoTTag, "- BC95G connected to MQTT-broker -");
		} else if (at_response[12] == '1') {
			LOGV(NBIoTTag, "- BC95G retrying to connect to MQTT-broker  -");
		} else if (at_response[12] == '2') {
			LOGV(NBIoTTag, "- BC95G failed to connect to MQTT-broker -");
		}

//+QMTOPEN: <tcpconnectID>,<result> only result is relevant
	} else if (c4 == 'O' && c5 == 'P' && c6 == 'E') {
		MQTT_error = true;
		if (at_response[12] == '-') {
			LOGV(NBIoTTag, 
					"- BC95G MQTT failed to open TCP connection to broker -");
		} else if (at_response[12] == '0') {
			MQTT_error = false;
			MQTT_connected = true;
			LOGV(NBIoTTag, "- BC95G MQTT TCP connection to broker established   -");
		} else if (at_response[12] == '1') {
			LOGV(NBIoTTag, "- BC95G MQTT wrong parameters in AT+QMTCONN -");
		} else if (at_response[12] == '2') {
			LOGV(NBIoTTag, "- BC95G MQTT identifier already occupied -");
		} else if (at_response[12] == '3') {
			LOGV(NBIoTTag, "- BC95G MQTT failed to activate PDP -");
		} else if (at_response[12] == '4') {
			LOGV(NBIoTTag, "- BC95G MQTT failed to parse domain name -");
		} else if (at_response[12] == '5') {
			LOGV(NBIoTTag, "- BC95G MQTT network disconnection error -");
		}

		//+QMTSTAT: <tcpconnectID>,<err_code> only err_code is relevant
	} else if (c4 == 'S' && c5 == 'T' && c6 == 'A') {
		MQTT_error = true;
		if (at_response[12] == '-') {
			LOGV(NBIoTTag, "- BC95G MQTT failed to open network -");
		} else if (at_response[12] == '0') {
			LOGV(NBIoTTag, "- BC95G MQTT retrying to connect to MQTT-broker  -");
		} else if (at_response[12] == '1') {
			LOGV(NBIoTTag, "- BC95G MQTT wrong parameters for QMTCONN -");
		} else if (at_response[12] == '2') {
			LOGV(NBIoTTag, "- BC95G MQTT identifier already occupied -");
		} else if (at_response[12] == '3') {
			LOGV(NBIoTTag, "- BC95G MQTT failed to activate PDP -");
		} else if (at_response[12] == '4') {
			LOGV(NBIoTTag, "- BC95G MQTT failed to parse domain name -");
		} else if (at_response[12] == '5') {
			LOGV(NBIoTTag, "- BC95G MQTT network disconnection error -");
		}
	}

	return ret;
}

/**
 * @brief extracts information from the response of AT-CMD "AT+CSQ" +CSQ: <rssi>,<ber>
 * @param at_response (char[]) one line received by the BC95G (that is assumedly a URC)
 * @retvalue signals if information could be extracted (true) or not (false)
 */
bool BC95G::Extract_Information_CSQ(char at_response[]) {
	bool ret = false; //+CSQ: <rssi>,<ber> is not URC, no further checks needed
	uint8_t rssiLocal = 0;
	/* As written in Quectel's AT Command Manual for the BC95G:
	 * "<ber> is currently not supported, and thus will always be 99."
	 * only the signal strength (<rssi>) will be extracted here
	 */
	char tmp[2];
	if (at_response[6] == ',') {
		tmp[0] = 0;
		tmp[1] = at_response[5];
		rssiLocal = 113 - 2 * (tmp[1] - '0');
		rssi[0] = '-';
		snprintf(rssi + 1, 4, "%d", rssiLocal);
		rssi[4] = '\0';
	} else if (at_response[6] == '9' && at_response[5] == '9') {
		LOGV(NBIoTTag, "RSSI: undetectable");
		return ret;
	} else {
		tmp[0] = at_response[5];
		tmp[1] = at_response[6];
		rssiLocal = 113 - 2 * atoi(tmp);
		rssi[0] = '-';
		snprintf(rssi + 1, 4, "%d", rssiLocal);
		rssi[4] = '\0';
		if (rssiLocal < 99)
			rssi[3] = '\0';
	}
	LOGV(NBIoTTag, "RSSI: %s dBm", rssi);
	return ret;
}

/**
 * @brief extracts information from NSOSTR URCs
 * @param at_response (char[]) one line received by the BC95G (that is assumedly a URC)
 * @retvalue signals if information could be extracted (true) or not (false)
 */
bool BC95G::Extract_Information_NSOSTR(char at_response[]) {

	bool ret = true;
	int lengthParts = 0;
	char* at_response_trimmed = strtrim(at_response);
	char** parts = strsplit_customchars(at_response_trimmed, &lengthParts, ",:", 2);
	if (lengthParts == 4 && strcmp(parts[3], "1") == 0)
	{
		udp_ack = true;
		udp_nack = false;
	}
	else if (lengthParts == 4 && strcmp(parts[3], "0") == 0)
	{
		udp_ack = false;
		udp_nack = true;
	}
	else
	{
		udp_ack = false;
		udp_nack = false;
	}

	for (int i = 0; i < lengthParts; i++) {
		free(parts[i]);
	}
	free(parts);
	free(at_response_trimmed);
	return ret;
}

/**
 * @brief Extracts information from NQSOS results
 * @param at_response (char[]) One line received by the BC95G 
 * @retvalue signals if information was a URC or not
 */
bool BC95G::Extract_Information_NQSOS(char at_response[]){
	bool ret = false;
	int lengthParts = 0;
	char** parts = strsplit_customchars(at_response, &lengthParts, ",:", 2);
	if (lengthParts == 3) {
		int sequence = atoi(parts[2]);
		if (sequence >= 0 && sequence <= 255){
			upstream_messages.set(sequence);
		}
	}

	for (int i = 0; i < lengthParts; i++) {
		free(parts[i]);
	}
	free(parts);

	return ret;
}

/**
 * @brief extracts information from the response of AT-CMD "AT+NUESTATS=RADIO"
 * 			unlike other BC95G_Extract_ functions, the at_response is given in global
 * 			linked list "head" and extracted from there. this is because the response is
 * 			over 13 lines long
 * @retvalue signals if information could be extracted (true) or not (false)
 */
bool BC95G::Extract_Information_NUESTATS() {
	bool ret = false;
	at_resp* cursor = head;
	uint8_t i;
	for(i=0;i<7;i++){
		if(cursor->line[i+28]!='\r'){
			BC95G_NUESTATS.signal_power[i] = cursor->line[i+28];
		}else{
			BC95G_NUESTATS.signal_power[i] = '\0';
			break;
		}
	}

	if(strcmp(BC95G_NUESTATS.signal_power, "-32768")==0) LOGV(NBIoTTag, "could not retrieve NUESTATS");

	cursor = cursor->next;
	for(i=0;i<7;i++){
		if(cursor->line[i+27]!='\r'){
			BC95G_NUESTATS.total_power[i] = cursor->line[i+27];
		}else{
			BC95G_NUESTATS.total_power[i] = '\0';
			break;
		}
	}

	cursor = cursor->next;
	for(i=0;i<7;i++){
		if(cursor->line[i+24]!='\r'){
			BC95G_NUESTATS.tx_power[i] = cursor->line[i+24];
		}else{
			BC95G_NUESTATS.tx_power[i] = '\0';
			break;
		}
	}

	cursor = cursor->next;
	for(i=0;i<7;i++){
		if(cursor->line[i+23]!='\r'){
			BC95G_NUESTATS.tx_time[i] = cursor->line[i+23];
		}else{
			BC95G_NUESTATS.tx_time[i] = '\0';
			break;
		}
	}

	cursor = cursor->next;
	for(i=0;i<7;i++){
		if(cursor->line[i+23]!='\r'){
			BC95G_NUESTATS.rx_time[i] = cursor->line[i+23];
		}else{
			BC95G_NUESTATS.rx_time[i] = '\0';
			break;
		}
	}

	cursor = cursor->next;
	for(i=0;i<11;i++){
		if(cursor->line[i+23]!='\r'){
			BC95G_NUESTATS.cell_ID[i] = cursor->line[i+23];
		}else{
			BC95G_NUESTATS.cell_ID[i] = '\0';
			break;
		}
	}

	cursor = cursor->next;
	for(i=0;i<4;i++){
		if(cursor->line[i+19]!='\r'){
			BC95G_NUESTATS.ecl[i] = cursor->line[i+19];
		}else{
			BC95G_NUESTATS.ecl[i] = '\0';
			break;
		}
	}

	cursor = cursor->next;
	for(i=0;i<7;i++){
		if(cursor->line[i+19]!='\r'){
			BC95G_NUESTATS.snr[i] = cursor->line[i+19];
		}else{
			BC95G_NUESTATS.snr[i] = '\0';
			break;
		}
	}

	cursor = cursor->next;
	for(i=0;i<11;i++){
		if(cursor->line[i+22]!='\r'){
			BC95G_NUESTATS.earfcn[i] = cursor->line[i+22];
		}else{
			BC95G_NUESTATS.earfcn[i] = '\0';
			break;
		}
	}
	cursor = cursor->next;
	for(i=0;i<6;i++){
		if(cursor->line[i+19]!='\r'){
			BC95G_NUESTATS.pci[i] = cursor->line[i+19];
		}else{
			BC95G_NUESTATS.pci[i] = '\0';
			break;
		}
	}
	cursor = cursor->next;
	for(i=0;i<7;i++){
		if(cursor->line[i+20]!='\r'){
			BC95G_NUESTATS.rsrq[i] = cursor->line[i+20];
		}else{
			BC95G_NUESTATS.rsrq[i] = '\0';
			break;
		}
	}
	cursor = cursor->next;
	for(i=0;i<2;i++){
		if(cursor->line[i+29]!='\r'){
			BC95G_NUESTATS.operator_mode[i] = cursor->line[i+29];
		}else{
			BC95G_NUESTATS.operator_mode[i] = '\0';
			break;
		}
	}
	cursor = cursor->next;
	for(i=0;i<4;i++){
		if(cursor->line[i+28]!='\r'){
			BC95G_NUESTATS.band[i] = cursor->line[i+28];
		}else{
			BC95G_NUESTATS.band[i] = '\0';
			break;
		}
	}

	cursor = head;
	LOGV(NBIoTTag, "%s", cursor->line);
	LOGV(NBIoTTag, "signal power: %s", BC95G_NUESTATS.signal_power);
	cursor = cursor->next;
	LOGV(NBIoTTag, "%s", cursor->line);
	LOGV(NBIoTTag, "total power: %s", BC95G_NUESTATS.total_power);
	cursor = cursor->next;
	LOGV(NBIoTTag, "%s", cursor->line);
	LOGV(NBIoTTag, "tx power: %s", BC95G_NUESTATS.tx_power);
	cursor = cursor->next;
	LOGV(NBIoTTag, "%s", cursor->line);
	LOGV(NBIoTTag, "tx time: %s", BC95G_NUESTATS.tx_time);
	cursor = cursor->next;
	LOGV(NBIoTTag, "%s", cursor->line);
	LOGV(NBIoTTag, "rx time: %s", BC95G_NUESTATS.rx_time);
	cursor = cursor->next;
	LOGV(NBIoTTag, "%s", cursor->line);
	LOGV(NBIoTTag, "cellID: %s", BC95G_NUESTATS.cell_ID);
	cursor = cursor->next;
	LOGV(NBIoTTag, "%s", cursor->line);
	LOGV(NBIoTTag, "ecl: %s", BC95G_NUESTATS.ecl);
	cursor = cursor->next;
	LOGV(NBIoTTag, "%s", cursor->line);
	LOGV(NBIoTTag, "snr: %s", BC95G_NUESTATS.snr);
	cursor = cursor->next;
	LOGV(NBIoTTag, "%s", cursor->line);
	LOGV(NBIoTTag, "earfcn: %s", BC95G_NUESTATS.earfcn);
	cursor = cursor->next;
	LOGV(NBIoTTag, "%s", cursor->line);
	LOGV(NBIoTTag, "pci: %s", BC95G_NUESTATS.pci);
	cursor = cursor->next;
	LOGV(NBIoTTag, "%s", cursor->line);
	LOGV(NBIoTTag, "rsrq: %s", BC95G_NUESTATS.rsrq);
	cursor = cursor->next;
	LOGV(NBIoTTag, "%s", cursor->line);
	LOGV(NBIoTTag, "operator_mode: %s", BC95G_NUESTATS.operator_mode);
	cursor = cursor->next;
	LOGV(NBIoTTag, "%s", cursor->line);
	LOGV(NBIoTTag, "band: %s", BC95G_NUESTATS.band);

	return ret; //TODO return value is not filled yet
}

/**
 * @brief Converts the periodic tau value of the UE into a time in seconds
 */
time_t BC95G::GetPeriodicTAU()
{
	time_t value = periodic_TAU & 0x1F;
	time_t unit = 0;
	switch (periodic_TAU >> 5)
	{
		case 0:
			unit = 10 * 60; // 10 minutes
			break;
		case 1:
			unit = 60 * 60; // 1 hour
			break;
		case 2:
			unit = 10 * 60 * 60; // 10 hours
			break;
		case 3:
			unit = 2; // 2 seconds
			break;
		case 4:
			unit = 30; // 30 seconds
			break;
		case 5:
			unit = 60; // 1 minute
			break;
		case 6:
			unit = 320 * 60 * 60; // 320 hours
			break;
		case 7:
		default:
			unit = 0; // timer is deactivated
			break;
	}
	return unit * value;
}

/**
 * @brief Converts the active time value of the UE into a time in seconds
 */
time_t BC95G::GetActiveTime()
{
	time_t value = active_time & 0x1F;
	time_t unit = 0;
	switch (active_time >> 5)
	{
		case 0:
			unit = 2; // 2 seconds
			break;
		case 1:
			unit = 60; // 1 minute
			break;
		case 2:
			unit = 10 * 60; // 10 minutes
			break;
		case 7:
		default:
			unit = 0; // timer is deactivated
			break;
	}
	return unit * value;
}

/**
 * @brief publishes a message via MQTT (Standard MQTT using TCP-IP)
 * @param message (char[]) mqtt payload to be published
 * @retvalue signals if message was published (true) or not (false)
 */
bool BC95G::MQTTPub(char broker_hostname[], char broker_port[],
		char mqtt_topic[], char message[]) {
	if (!registered) {
		Connect();
		if (!registered) {
			LOGV(NBIoTTag, 
					"Error: BC95G not registered to network after 2nd attempt");
			return false;
		}
	}
	uint8_t attempts = 0;
	AT_Command("AT+QMTCFG=\"version\",0,4", 0, 1000);
	AT_Command("AT+QMTCFG=\"keepalive\",0,300", 0, 1000);
	//AT_Command("AT+QMTCFG=\"session\",0,1", 0, 1000);
	//AT_Command("AT+QMTCFG=\"timeout\",0,60,3,1", 0, 1000);
	LOGV(NBIoTTag, "- Connecting to %s:%s -", broker_hostname, broker_port);
	char at_qmtopen[100] = "AT+QMTOPEN=0,";
	strcat(at_qmtopen, "\"");
	strcat(at_qmtopen, broker_hostname);
	strcat(at_qmtopen, "\",");
	strcat(at_qmtopen, broker_port);
	AT_Command(at_qmtopen, 0, 1000);
	while (!MQTT_error && !MQTT_connected && attempts < 400) {
		HAL_Delay(50);
		attempts++;
	}
	if (MQTT_error) {
		AT_Command("AT+QMTDISC=0", 0, 1000);
		AT_Command("AT+QMTCLOSE=0", 0, 1000);
		return false;
	} else if (!MQTT_connected) {
		LOGV(NBIoTTag, 
				"- BC95G couldn't open a connection to the MQTT-broker after %lf seconds of waiting -",
				attempts * 0.05);
		AT_Command("AT+QMTDISC=0", 0, 1000);
		AT_Command("AT+QMTCLOSE=0", 0, 1000);
		return false;
	}
	AT_Command("AT+QMTCONN=0,\"BC95G1\",\"Test\",\"Test\"", 0,
			1000);
	attempts = 0;
	MQTT_error = false;
	MQTT_connected = false;
	while (!MQTT_error && !MQTT_connected && attempts < 400) {
		HAL_Delay(50);
		attempts++;
	}
	if (MQTT_error) {
		AT_Command("AT+QMTDISC=0", 0, 1000);
		AT_Command("AT+QMTCLOSE=0", 0, 1000);
		return false;
	} else if (!MQTT_connected) {
		LOGV(NBIoTTag, 
				"- BC95G couldn't open a connection to the MQTT-broker after %lf seconds of waiting -",
				attempts * 0.05);
		AT_Command("AT+QMTDISC=0", 0, 1000);
		AT_Command("AT+QMTCLOSE=0", 0, 1000);
		return false;
	}
	char at_qmtpub[200];
	at_qmtpub[0]='\0';
	strcat(at_qmtpub,"AT+QMTPUB=0,0,0,0,\"");
	strcat(at_qmtpub,mqtt_topic);
	strcat(at_qmtpub,"\"");
	LOGV(NBIoTTag, "- Publishing in Topic %s-", mqtt_topic);
	LOGV(NBIoTTag, "at_qmtpub: %s",at_qmtpub);
	AT_Command(at_qmtpub, 0, 1000);
	HAL_UART_Transmit(huart, (uint8_t *) message, strlen(message), 1000);
	//LOGV(NBIoTTag, "- Disconnecting from the MQTT Broker -");
	while (!MQTT_error && !MQTT_published && attempts < 40) {
		HAL_Delay(500);
		attempts++;
	}
	AT_Command("AT+QMTDISC=0", 0, 1000);
	AT_Command("AT+QMTCLOSE=0", 0, 1000);
	return true;
}

/**
 * @brief Opens a UDP socket for sending UDP datagram
 * @retvalue signals if socket was opened (true) or not (false)
 */
bool BC95G::OpenUDPSocket()
{
	if(!registered || !attached) {
		LOGE("NB-IoT", "BC95G not connected");
		return false;
	}
	/* open the socket for datagrams (DGRAM) using the UDP protocol (17)*/
	if (!AT_Command("AT+NSOCR=DGRAM,17,49153,1", 1, 300)) {
		if (head->line[0] != '1') {
			LOGE("NB-IoT", "UDP socket could not be opened");
			return false;
		} else {
			LOGV("NB-IoT", "UDP socket opened");
			socketOpen = true;
			return true;
		}
	} else {
		LOGE("NB-IoT", "UDP socket could not be opened");
		return false;
	}
}

/**
 * @brief Closes a UDP socket for sending UDP datagram
 * @retvalue signals if sockets was closed (true) or not (false)
 */
bool BC95G::CloseUDPSocket() {
	// Wait for all messages to be send
	do {
		upstream_messages.reset();
		AT_Command("AT+NQSOS?", 0, 300);
	} while (!upstream_messages.none());


	if (!AT_Command("AT+NSOCL=1", 0, 300)) {
		LOGV("NB-IoT", "Closed UDP socket");	
		socketOpen = false;
		return true;
	}
	else {
		LOGE("NB-IoT", "Not closed UDP socket");
		return false;
	}
}

uint8_t sequenceNumber = 1;

/**
 * @brief sends a UDP datagram to desired socket
 * @param message (char[]) payload to be enveloped in the UDP datagram must be a hexstring
 * @param destIP (char[]) destination IP
 * @param destPort (char[]) destination port
 * @retvalue signals if udp message was sent (true) or not (false)
 */
bool BC95G::UDPSend(char payload[], char destIP[], char destPort[]) {
	if (!registered || !attached) {
		LOGE("NB-IoT", "BC95G not connected");
		return false;
	}
	if (!socketOpen)
	{
		LOGE("NB-IoT", "UDP socket not open!");
	}
	/* the payload is a hexstring, if the number of chars is odd then somewhere in the message
	 * a byte would be incomplete (4bits) since 1 byte is 2 hex-chars*/
	if (strlen(payload) % 2 != 0) {
		LOGE("NB-IoT", "UDP message has an odd number of chars");
		return false;
	}
	/* AT-Command NSOST needs the following information
	 * AT+NSOST=1,<destIP>,<destPort>,<messagelength>,<message>,<sequence>
	 * the length (of char at_nsost[]) consists of the AT+NSOST=1, the string length of
	 * port, IP and payload as well as the commas and string termination '\0' 
	 * The sequence allows to send (give to the UE) multiple packets at once.
	 */
	uint32_t at_nsost_len = 24 + strlen(destIP) + strlen(destPort)
			+ strlen(payload);
	char at_nsost[at_nsost_len];
	at_nsost[0] = 0;
	strcat(at_nsost, "AT+NSOST=1,");
	strcat(at_nsost, destIP);
	strcat(at_nsost, ",");
	strcat(at_nsost, destPort);
	strcat(at_nsost, ",");
	char message_length[11];
	snprintf(message_length, sizeof(message_length), "%d", (int)(strlen(payload) / 2));
	strcat(at_nsost, message_length);
	strcat(at_nsost, ",");
	strcat(at_nsost, payload);
	strcat(at_nsost, ",");

	// We can increase the sequenceNumber with ++ because it will automatically
	// overflow. The usage of an already used sequence number is a problem when 
	// the frame with this packets hasn't been sent. This results then in the 
	// CME Error 332 Network Timeout. This shouldn't normally happen,
	// because this functions waits for an ACK.
	char sequence[4];
	snprintf(sequence, sizeof(sequence), "%d", sequenceNumber++);
	strcat(at_nsost, sequence);
	if (sequenceNumber == 0 || sequenceNumber > 255) {
		sequenceNumber = 1;
	}

	udp_ack = false;
	udp_nack = false;
	AT_Command(at_nsost, 1, 1000);
	if (head->line[0] == '1') {
		LOGI("NB-IoT", "UDP packet sent");
	} else {
		LOGE("NB-IoT", "UDP packet could not be sent!");
		return false;
	}
	uint8_t attempts = 0;
	while (!udp_ack && !udp_nack && attempts < 200) { // 200attempts*100ms delay=20s timeout
		ProcessMessageQueue();
		HAL_Delay(100);
		attempts++;
	}
	if (udp_ack) {
		LOGI("NB-IoT", "UDP datagram has been sent to destination");
	} else {
		LOGE("NB-IoT", "Error while sending UDP datagram");
		return false;
	}
	return true;
}

/**
 * @brief builds a PUBLISH message with QoS-1 (negative one) and sends it via UDP
 * 		  uses a predefined topic ID by default, no retain will or cleansession active
 * @param destIP (char[]) destination IP for the UDP transmission
 * @param destPort (char[]) destination Port for the UDP transmission
 * @param topicID (uint16_t) topicID field of the MQTT-SN message, predefined in the gateway - must be configured and known
 * @param msgID (uint16_t) msgID field of the MQTT-SN message
 * @param message (* char) message to be sent - must be ASCII symbols of the sets a-z, A-Z or 0-9
 * @param messageLength length of the message ot be sent in bytes
 * @retval (bool) signals if the message has been sent (true) or not (false)
 *
 * structure (fields) of the PUBLISH message with QoS -1 (Minus one)
 * byte#:|-0----|-1-----|-2---|-3-|-4-|-5-|-6-|-7-... (upto byte# 65535)
 * field:|Length|MsgType|Flags|TopicID|-MsgID-|Data... (max. 65528 bytes of data)
 * Note: The length field takes up bytes 0-2 if the total size exceeds 255 bytes, MsgType is then byte 3 etc.
 * 		 This field depends on all the other content, especially the data (which is of variable size)
 * 		 so the length is the first field computed, then the message is put together
 */
bool BC95G::MQTTSN_PublishQoSMinus1(char destIP[], char destPort[], 
		uint16_t topicID, uint16_t msgID, char *message,
		uint16_t messageLength) {
	uint16_t msglength = 0; //length in bytes of the char array of the message, not to be confused with the length field of MQTTSN
	msglength = 6; //the message always includes 6 bytes for MsgType|Flags|TopicID|MsgID
	/* generating the parts of the message */
	char flagbyte[3];
	flagbyte[2] = '\0';
	MQTTSN_Flagbyte(flagbyte, false, 0b11, false, false, false, 0b01);
	/* a conversion from int to char and ascii to hex is done because the BC95/M95
	 * needs a hexadecimal string as an input for the AT commands transmitted via UART
	 * specifically the UDP datagram's payload needs to be a hexstring
	 * MQTTSN_Flagbyte delivers a hexstring, other parameters need to be converted */
	char topicID_hex[5], msgID_hex[5]; //topicID and msgID are taken from the input parameters of the function
	topicID_hex[4] = '\0';
	msgID_hex[4] = '\0';
	BYTE_to_HEX((0x00 | (topicID << 8)), topicID_hex);
	BYTE_to_HEX((0x00 | topicID), topicID_hex + 2);
	BYTE_to_HEX((0x00 | (msgID << 8)), msgID_hex);
	BYTE_to_HEX((0x00 | msgID), msgID_hex + 2);
	/* conversion of the mqtt-sn payload from ascii to hexstring -> size of the char array is doubled */
	char message_hex[messageLength * 2 + 1]; //
	message_hex[0] = 0;
	msglength += ARRAY_to_HEXSTRING(message, messageLength, message_hex,
			messageLength * 2 + 1);
	/* now the whole mqtt-sn message length in bytes (without the length field)
	 * is stored in msglength and the length field can be generated */
	char lengthfield[7];
	lengthfield[6] = '\0';
	msglength += MQTTSN_Messagelength(lengthfield, msglength);
	/* MQTT-SN package fields are all available and can be used to form the final message*/
	char msg_hex[(msglength * 2) + 1]; //length is doubled because 1ascii=2hex numbers, +1 for string termination '\0'
	msg_hex[0] = 0;
	strcat(msg_hex, lengthfield);
	strcat(msg_hex, PUBLISH); //MsgType-defines are to be found in mqttsn.h, here PUBLISH ("04") is used
	strcat(msg_hex, flagbyte);
	strcat(msg_hex, topicID_hex);
	strcat(msg_hex, msgID_hex);
	strcat(msg_hex, message_hex);
	msg_hex[msglength * 2] = '\0';
	/* sending the message via UDP through given socket*/
	return UDPSend(msg_hex, destIP, destPort);
}
