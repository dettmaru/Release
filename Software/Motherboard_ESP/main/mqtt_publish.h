/*
 * mqtt_publish.h
 *
 *  Created on: Okt 5, 2018
 *      Author: Daniel Lohmann
 */
#ifndef MQTT_PUBLISH_H
#define MQTT_PUBLISH_H

#include <stdint.h>

extern char* CONF_WLANSSID;
extern char* CONF_WLANPassword;
extern char* CONF_MQTTServerUrl;
extern char* CONF_MQTTTopic;
extern char* CONF_MQTTUsername;
extern char* CONF_MQTTPassword;
extern char* CONF_MQTTIssuerCertificate;
extern char* CONF_MQTTClientID;

void MQTT_Connect();
int MQTT_Publish(uint8_t* data, uint16_t length);
void MQTT_Disconnect();

#endif // MQTT_PUBLISH_H