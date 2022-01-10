/*
 * matt_publish.h
 *
 *  Created on: Okt 5, 2018
 *      Author: Daniel Lohmann
 */
#include <stdlib.h>
#include "mqtt_publish.h"
#include "wifi.h"
#include "MQTTClient.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_wpa2.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"

char* CONF_MQTTServerUrl = NULL;
char* CONF_MQTTTopic = NULL;
char* CONF_MQTTUsername = NULL;
char* CONF_MQTTPassword = NULL;
char* CONF_MQTTIssuerCertificate = NULL;
char* CONF_MQTTClientID = NULL;


#define MQTT_PORT 8883
#define MQTT_BUF_SIZE 1000
#define MQTT_WEBSOCKET 0  // 0=no 1=yes*/

static unsigned char mqtt_sendBuf[MQTT_BUF_SIZE];
static unsigned char mqtt_readBuf[MQTT_BUF_SIZE];

static const char *TAG = "MQTTS";

Network network;
MQTTClient client;
extern const char *server_root_cert;

bool connected = false;
void MQTT_Connect()
{
    if (connected)
    {
        return;
    }
    connected = true;

    int ret = 0;
    uint8_t chipid[6];
    esp_efuse_mac_get_default(chipid);
    ESP_LOGI("phy", "%02x:%02x:%02x:%02x:%02x:%02x\n",chipid[0], chipid[1], chipid[2], chipid[3], chipid[4], chipid[5]);

    /* Wait for the callback to set the CONNECTED_BIT in the
        event group.
    */
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
                        false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to AP");

    ESP_LOGI(TAG, "Start MQTT Task ...");

    ESP_LOGI(TAG, "Certificate: %s", CONF_MQTTIssuerCertificate);

    NetworkInit(&network);
    network.websocket = MQTT_WEBSOCKET;

    ESP_LOGI(TAG,"NetworkConnect %s:%d ...", CONF_MQTTServerUrl, MQTT_PORT);
    char* certificate = server_root_cert;
    if (CONF_MQTTIssuerCertificate != NULL)
    {
        certificate = CONF_MQTTIssuerCertificate;
    }
    NetworkConnect(&network, CONF_MQTTServerUrl, MQTT_PORT, certificate);
    ESP_LOGI(TAG,"MQTTClientInit  ...");
    MQTTClientInit(&client, &network,
        2000,           // command_timeout_ms
        mqtt_sendBuf,   // sendbuf,
        MQTT_BUF_SIZE,  // sendbuf_size,
        mqtt_readBuf,   // readbuf,
        MQTT_BUF_SIZE   // readbuf_size
    ); 

    MQTTString clientId = MQTTString_initializer;
    clientId.cstring = CONF_MQTTClientID;
    
    MQTTString userName = MQTTString_initializer;
    userName.cstring = CONF_MQTTUsername;

    MQTTString password = MQTTString_initializer;
    password.cstring = CONF_MQTTPassword;

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID          = clientId;
    data.willFlag          = 0;
    data.MQTTVersion       = 4; // 3 = 3.1 4 = 3.1.1
    data.keepAliveInterval = 60;   // In seconds
    data.cleansession      = 1;     // Every time a new session
    if (CONF_MQTTPassword != NULL && 
        CONF_MQTTUsername != NULL &&
        strlen(CONF_MQTTPassword) > 0 &&
        strlen(CONF_MQTTUsername) > 0)
    {
        ESP_LOGI(TAG, "Username: %s", CONF_MQTTUsername);
        ESP_LOGI(TAG, "Password: %s", CONF_MQTTPassword);
        data.username = userName;
        data.password = password;
    }
    else
    {
        ESP_LOGI(TAG, "Using no username and password!");
    }

    ESP_LOGI(TAG, "ClientID: %s", CONF_MQTTClientID);
    ESP_LOGI(TAG, "MQTTConnect  ...");
    ret = MQTTConnect(&client, &data);
    if (ret != SUCCESS) {
        char buffer[100];
        strcpy(buffer, "");
        switch (ret)
        {
            case FAILURE:
                strcpy(buffer, "Connection Failure (no CONNACK from server)");
                break;
            case 1:
                strcpy(buffer, "Connection Refused, unacceptable protocol version!");
                break;
            case 2:
                strcpy(buffer, "Connection Refused, identifier rejected!");
                break;
            case 3:
                strcpy(buffer, "Connection Refused, Server unavailable!");
                break;
            case 4:
                strcpy(buffer, "Connection Refused, bad user name or password!");
                break;
            case 5:
                strcpy(buffer, "Connection Refused, not authorized!");
                break;
            default:
                break;
        }

        ESP_LOGI(TAG, "MQTTConnect not SUCCESS: %d => %s", ret, buffer);
    }
}

int MQTT_Publish(uint8_t* data, uint16_t length)
{
    int ret = 0;

    MQTTMessage message;
    message.qos = QOS2;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)(data);
    message.payloadlen = length;

    ESP_LOGI(TAG, "Topic: %s", CONF_MQTTTopic);
    ret = MQTTPublish(&client, CONF_MQTTTopic, &message);
    if (ret != SUCCESS) {
        ESP_LOGW(TAG, "MQTTPublish not SUCCESS: %d", ret);
        // TODO goto exit;
    }
    else {
        ESP_LOGD(TAG, "MQTTPublish SUCCESS");
    }
    return ret;
}

void MQTT_Disconnect()
{
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
    ESP_LOGI(TAG, "Finished Sending, Disconnected");
}