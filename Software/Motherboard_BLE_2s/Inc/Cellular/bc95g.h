/**
 * @file bc95g.h
 *
 *  Created on: Nov 29, 2018
 *      Author: Joschka Wirges, Daniel Lohmann
 */

#include <stdbool.h>
#include <stdio.h>
#include <bitset>

#ifndef BC95_H_
#define BC95_H_

/**
 *  @brief stores information on the state of the NB-IoT-module Quectel BC95G
 */
typedef struct BC95G_Status{
	/**
	 * @brief signal power in centibels
	 */
	char signal_power[7];
	/**
	 * @brief total power in centibels
	 */
	char total_power[7];
	/**
	 * @brief current tx power level in centibels
	 */
	char tx_power[7];
	/**
	 * @brief total tx time since last reboot in milliseconds
	 */
	char tx_time[7];
	/**
	 * @brief total rx time since last reboot in milliseconds
	 */
	char rx_time[7];
	/**
	 * @brief Last seen SIB1 cell ID
	 */
	char cell_ID[11];
	/**
	 * @brief Last ECL value
	 */
	char ecl[4];
	/**
	 *	@brief Last SNR value
	 */
	char snr[7];
	/**
	 *	@brief last EARFCN value
	 */
	char earfcn[11];
	/**
	 *	@brief Last PCI value
	 */
	char pci[6];
	/**
	 *	@brief Reference Signal Received Quality in centibels
	 */
	char rsrq[7];
	/**
	 *	@brief NB-IoT SIB1 operator mode
	 */
	char operator_mode[2];
	/**
	 *	@brief NB-IoT frequency band (LTE band)
	 */
	char band[4];
}BC95G_Status;

/**
 *  @brief stores information on the state of the NB-IoT-module Quectel BC95G
 */
class BC95G{
public:
	/**
	 * @brief pointer to the BC95G UART interface
	 */
	UART_HandleTypeDef *huart;
	/**
	 * @brief stores the current RSSI with 0-31 and 99 as possible values
 	 * 		  0=-113dBm or less,1=-111dBm, 2...30=-109...-53dBm and 99=not known/undetectable
 	 * 		  issued with AT+CSQ(=rssi,ber)
	 */
	char rssi[5];
	/**
	 * @brief Count of lines that have been received
	 */
	uint8_t lines_received;
	/**
	 * @brief stores any latest CME/CMS-error the device has issued as a response to an AT-command
 	 *        a list of all CME/CMS errors can be found in the appendix of the AT-Commands Manual
	 */
	char cme_cms_error[4];
	/**
	 * @brief IMSI of the SIM connected to the M95 issued with AT+CIMI
	 */
	char imsi[33];
	/**
	 * @brief stores the firmware version of the BC95G which implies,
 	 * 		  if certain operations are possible to execute (MQTT,TLS) issued with AT+QGMR?
	 */
	char firmware_ver[20];
	/**
	 * @brief stores the operator of the attached cellular network
 	 * 		  issued with AT+COPS?
	 */
	char mobile_operator[30];
	/**
	 * @brief stores the last received URC
	 */
	char URC[100];
	/**
	 * @brief stores whether a given sequence number is queued for transmission
	 */
	std::bitset<256> upstream_messages;
	/**
	 * @brief ERROR received from the module (final response)
	 */
	volatile bool respERR;
	/**
	 * @brief OK received from the module (final response)
	 */
	volatile bool respOK;
	/**
	 * @brief CME error code received from the module (final response)
	 */
	volatile bool respCME;
	/**
	 * @brief CMS error code from the module (final response)
	 */
	volatile bool respCMS;
	/**
	 * @brief flag to signal a received URC
	 */
	volatile bool URC_received;
	/**
	 * @brief flag to show registration status of module to operator
	 */
	volatile bool registered;
	/**
     * @brief flag to show the attachment status of module to cell
 	 *        note to attached, registered: a module registers with its sim to an operator, it
 	 *        may be possible that after successful registration no attachment is made because the
 	 *        cell is overloaded or for other reasons->no data communication is possible then, retry or wait
	 */
	volatile bool attached;
	/**
	
	 */
	volatile bool socketOpen;
	/**
	 * @brief flag to show the arrival of a udp packet at destination
	 */
	volatile bool udp_ack;
	/**
	 * @brief flag to show the error of the transmission of the last sent udp packet
	 */
	volatile bool udp_nack;
	/**
	 * @brief flag to show power saving mode status of the module
	 */
	volatile bool PSM_active;
	/**
	 * @brief periodic tau value of UE
	 */
	volatile uint8_t periodic_TAU;
	/**
	 * @brief active_time value of UE
	 */
	volatile uint8_t active_time;
	/**
	 * @brief flag to signal an error while connecting to MQTT broker/publishing
	 */
	volatile bool MQTT_error;
	/**
	 * @brief flag to signal connection to the MQTT broker
	 */
	volatile bool MQTT_connected;
	/**
     * @brief flag to signal a successful publishing
	 */
	volatile bool MQTT_published;
	/**
	 * @brief data type holding response to AT command AT+NUESTATS
	 */
	BC95G_Status BC95G_NUESTATS;


	/*BC95G Function Prototypes*/
	bool Init(UART_HandleTypeDef *huart);
	bool ReInit(UART_HandleTypeDef* huart);
	void DeInit(void);
	void Reset(void);
	bool AT_Command(char cmd[], uint8_t lines_to_receive, uint32_t timeout);
	bool Final_Response();
	void ProcessMessageQueue();
	void Decode_AT_Resp(char at_response[]);
	bool Connect();
	bool Decode_URC(char at_response[]);
	bool Wait_For_Final_Response(uint32_t timeout);
	bool Extract_Information_CEREG(char at_response[]);
	bool Extract_Information_CME(char at_response[]);
	bool Extract_Information_CMS(char at_response[]);
	bool Extract_Information_NPSMR(char at_response[]);
	bool Extract_Information_CPSMS(char at_response[]);
	bool Extract_Information_CSCON(char at_response[]);
	bool Extract_Information_QMT(char at_response[]);
	bool Extract_Information_CSQ(char at_response[]);
	bool Extract_Information_NSOSTR(char at_response[]);
	bool Extract_Information_NQSOS(char at_response[]);
	bool Extract_Information_NUESTATS();
	bool MQTTPub(char broker_hostname[], char broker_port[], char message[],
			char mqtt_topic[]);
	bool OpenUDPSocket();
	bool CloseUDPSocket();
	bool UDPSend(char message[], char destIP[], char destPort[]);
	bool MQTTSN_PublishQoSMinus1(char destIP[], char destPort[], 
			uint16_t topicID, uint16_t msgID, char message[], 
			uint16_t messsageLength);

	time_t GetPeriodicTAU();
	time_t GetActiveTime();

};

extern BC95G hBC95G;
#endif /* BC95_H_ */
