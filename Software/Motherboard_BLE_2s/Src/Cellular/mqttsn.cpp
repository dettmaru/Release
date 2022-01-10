/**
 * @file mqttsn.c
 *
 *  Created on: Mar 11, 2019
 *      Author: Joschka Wirges, Daniel Lohmann
 */

#include "main.h"
#include "stm32l4xx_hal.h"
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include "mqttsn.h"
#include <string.h>
/**
 * @brief creates the flagbyte for MQTTSN-messages, returns 2 chars as hexadecimal string representation
 *        of the flagbyte
 * see section 5.3.4 of mqtt.org/new/wp-content/uploads/2009/06/MQTT-SN_spec_v1.2.pdf
 *
 * structure of the byte:
 * bit#:|-7-|-6--|-5--|-4----|-3--|-2----------|-1----------|-0----------|
 * flag:|DUP|QoS1|QoS2|Retain|Will|CleanSession|TopicIDType1|TopicIDType2|
 *
 * @param flagbyte (*char) return pointer for the flagbyte of size 8 bit (1 char)
 * @param DUP (bool) DUP flag
 * @param QoS (uint8_t) QoS-Level 0, 1 and 2 are coded 0b00, 0b01 and 0b10; QoS-Level -1 is 0b11
 * @param retain (bool) retain flag
 * @param will (bool) will flag
 * @param cleansession (bool) cleansession flag
 * @param topicIDtype (uint8_t) type of topic: predefined topicID 0b01, short topic name (string) 0b11 , normal topic ID 0b00
 * 		  predefined topicIDs need to be known by the MQTTSN gateway!
 */
void MQTTSN_Flagbyte(char *flagbyte, bool DUP, uint8_t QoS, bool retain, bool will, bool cleansession, uint8_t topicIDtype){
 uint8_t tmp = 0x00; //empty flagbyte
 if(DUP) tmp = (tmp | 0x80);
 tmp = (tmp | (QoS<<5));
 if(retain) tmp = (tmp | 0x10);
 if(will) tmp = (tmp | 0x08);
 if(cleansession) tmp = (tmp | 0x04);
 tmp = (tmp| topicIDtype); //final decimal notation of the flagbyte is now saved in tmp
 BYTE_to_HEX(tmp, flagbyte); //convert to hexadecimal notation (2 chars) and return via reference pointer "flagbyte"
}


/**
 * @brief creates the length field for the MQTT-SN message header, returns 3 or 7 chars as hexadecimal string representation
 *        of the field. the length field uses 1 byte (2 chars as return value) if the message is smaller than 256 bytes
 *        if it is bigger it uses 3 bytes (6 chars as return value) maximum size is 65535 bytes
 * see section 5.2.1 of mqtt.org/new/wp-content/uploads/2009/06/MQTT-SN_spec_v1.2.pdf
 *
 * @param lengthfield (*char) return pointer for the length field
 * @param msglength (uint16_t) length of the whole mqttsn message (without length field) in bytes
 * @retvalue returns the size of the length field in bytes (1 or 3 bytes) or -1 for an error (too large message)
 */
uint8_t MQTTSN_Messagelength(char *lengthfield, uint16_t msglength){
	uint8_t byte1 = 0x00;
	if (msglength<255){//length can be coded in 1 byte
		msglength++; //length field increases the total amount by 1 byte
		byte1 = byte1 | msglength;
		BYTE_to_HEX(byte1, lengthfield);
		lengthfield[2] = '\0';
		return 1;
	/* check how many bytes are needed for the length field and if the max length is exceeded */
	}else if((msglength>255) && (msglength<65532)){
		/* if the length exceeds 256 but is below 65535, the length field carries 3 bytes.
		 * the first byte signals that 3 bytes are needed, it is coded 0x01
		 * the second and third byte carry the total message length in bytes (including the 3 bytes of the length field)
		 */
		msglength = msglength + 3; //length field increases the total amount by 3 bytes
		byte1 = 0x01;
		uint8_t byte2 = 0x00 | (msglength>>8);
		uint8_t byte3 = 0x00 | msglength;
		BYTE_to_HEX(byte1, lengthfield);
		BYTE_to_HEX(byte2, lengthfield+2);
		BYTE_to_HEX(byte3, lengthfield+4);
		lengthfield[6] = '\0';
		return 3;
	}else{
		printf("Error: MQTTSN message not sent, maximum size of MQTTSN message (65535 bytes) exceeded\r\n");
		return -1;
	}
}


/**
 * @brief converts one byte to its hexadecimal value as a string (2 chars)
 * 		  can also be used to convert char to its hexvalue as a string (2 chars)
 * @param byte (char) byte to be converted
 * @param hex (*char) output pointer for the resulting two characters
 */
void BYTE_to_HEX(char byte, char *hex)
{
	uint8_t msb = byte >> 4;
	uint8_t lsb = byte & 0xF;
	*hex       = msb + '0' + (7 * (msb / 10));
	*(hex + 1) = lsb  + '0' + (7 * (lsb  / 10));
}

/**
 * @brief converts a byte array to its hexadecimal value as a string (doubles 
 * 		  the length of the input array)
 * @param string (*char) pointer to byte array that is to be converted
 * @param str_len length of the byte array
 * @param hexstring (*char) output pointer for the resulting hexadecimal string
 * @param maxlength (int) maximum length for the output variable to avoid memory overflow
 * @retval (int) length of the resulting hexadecimal string
 */
uint16_t ARRAY_to_HEXSTRING(const char * string, const uint16_t str_len, 
	char * hexstring, uint16_t maxlength)
{
	uint16_t i;
	char * ptr = hexstring;

	if (str_len * 2 > maxlength - 1) {
		return -1;
	}

	for (i = 0; i < str_len; i++) {
		BYTE_to_HEX(string[i], ptr);
		ptr += 2;
	}

	*ptr = '\0';

	return i;
}
