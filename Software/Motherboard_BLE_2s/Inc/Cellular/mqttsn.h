/**
 * @file mqttsn.h
 *
 *  Created on: Mar 11, 2019
 *      Author: Joschka Wirges, Daniel Lohmann
 */

#ifndef MQTTSN_H_
#define MQTTSN_H_
#include <stdbool.h>
#include <stdio.h>

// Messagetypes
#define PUBLISH "0C\0"
#define CONNECT "04\0"


/* MQTTSN Function Prototypes */
void MQTTSN_Flagbyte(char *flagbyte, bool DUP, uint8_t QoS, bool retain, bool will, bool cleansession, uint8_t topicIDtype);
uint8_t MQTTSN_Messagelength(char *lengthfield, uint16_t msglength);
void BYTE_to_HEX(char ascii, char *hex);
uint16_t ARRAY_to_HEXSTRING(const char * string, const uint16_t str_len, char * hexstring, uint16_t maxlength);

#endif /** MQTTSN_H_ */
