/*
 * uart_callback.c
 *
 *  Created on: May 22, 2019
 *      Author: Joschka Wirges, Daniel Lohmann
 */

#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"
#include "a2235.h"
#include "m95.h"
#include "bc95g.h"
#include "linkedlist.h"
#include "mqttsn.h"
#include "usart.h"
#include <string.h>
#include <queue>

std::queue<char*> bc95gQueue;
std::queue<char*>* bc95gQueuePtr = &bc95gQueue;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	static char buf_huart1[100]; //reception buffers for all uart-interfaces
	static char buf_huart2[100];
	static char buf_huart3[100];
	static uint16_t cnt_huart1 = 0;  //counter variable for the uart-buffers
	static uint16_t cnt_huart2 = 0;
	static uint16_t cnt_huart3 = 0;
	static volatile char readChar; //currently received char
	static bool newlineflag_huart2 = false;
	static bool newlineflag_huart3 = false;

	if (huart == &huart2) { //huart2 is the Quectel M95's UART interface (2G/GPRS module)
		__HAL_UART_FLUSH_DRREGISTER(&huart2);
		readChar = (char) *huart2.pRxBuffPtr;
		//message ongoing, write incoming byte to buf
		if (newlineflag_huart2) {
			buf_huart2[cnt_huart2] = readChar;
			cnt_huart2++;
		}
		// received \n (LF) signals the start or end of a message
		if (readChar == '\n') {
			//message ends here if true
			if (newlineflag_huart2) {
				/*decode and process the message*/
				M95_Decode_AT_Resp(&hM95, buf_huart2);
				newlineflag_huart2 = false; //reset newlineflag for the next line
				memset(buf_huart2, 0, sizeof buf_huart2); //set the whole buffer to 0
				cnt_huart2 = 0; //reset counter for next message
			} else { //message starts if else
				newlineflag_huart2 = true;
			}
		}
	} //end if UART 2G

	if (huart == &huart3) { //huart3 is the Quectel BC95G's UART interface (NB-IoT module)
		__HAL_UART_FLUSH_DRREGISTER(&huart3);
		readChar = (char) *huart3.pRxBuffPtr;
		//printf("%c",readChar);
		//message ongoing, write incoming byte to buf
		if (newlineflag_huart3) {
			buf_huart3[cnt_huart3] = readChar;
			cnt_huart3++;
		}
		// received \n (LF) signals the start or end of a message
		if (readChar == '\n') {
			//message ends here if true
			if (newlineflag_huart3) {
				/*decode and process the message*/
				if (buf_huart3[0] != '\r') { //check if the message is empty - if so, discard the message
					bc95gQueuePtr->push(strdup(buf_huart3));
					newlineflag_huart3 = false; //reset newlineflag for the next message reception
				}
				memset(buf_huart3, 0, sizeof buf_huart3); //set the whole buffer to 0 for the next message
				cnt_huart3 = 0; //reset counter for next message
			} else { //message starts if else
				newlineflag_huart3 = true;
			}
		}
	} //end if UART NBIOT

	if (huart == &huart1) { //huart1 is the Maestro A2235-H's UART interface (GPS module)
		__HAL_UART_FLUSH_DRREGISTER(&huart1);
		uint8_t getRMC = 0;
		const char GPRMC[] = { 'G', 'P', 'R', 'M', 'C' };
		bool is_GPRMC = true;
		char end = '\n';
		char char_return = '\r';
		char start = '$';
		char readChar = (char) *huart1.pRxBuffPtr;
		//printf("%c",readChar);
		if (readChar == start) {
			// Message starts here
			cnt_huart1 = 0;
		} else if (readChar == char_return) {
			// Message Complete, check for RMC message...
			for (uint8_t j = 0; j < 5; j++) {
				if (buf_huart1[j] != GPRMC[j]) {
					is_GPRMC = false;
				}
			}
			if (is_GPRMC) {
				// Incoming message is a RMC message!
				char rmcMessage[cnt_huart1];
				for (uint8_t i = 0; i < cnt_huart1; i++) {
					printf("%c", buf_huart1[i]);
					rmcMessage[i] = buf_huart1[i];
				}
				printf("\r\n");
				// Extract Content of RMC and store into hGPS object
				getRMC = hGPS.ProcessNMEA(rmcMessage);
			}
		} else if (readChar == end) {
			cnt_huart1 = 0;
		} else {
			// Check for overflow
			if (cnt_huart1 <= 85) {
				buf_huart1[cnt_huart1] = readChar;
				cnt_huart1++;
			} else {
				//Overflow!
				cnt_huart1 = 0;
			}
		}
	} //end if UART GPS
}
