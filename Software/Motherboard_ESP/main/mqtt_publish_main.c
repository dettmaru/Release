/* MQTT Example using mbedTLS sockets
 *
 * Contacts the howsmyssl.com API via TLS v1.2 and reads a JSON
 * response.
 *
 * Adapted from the ssl_client1 example in mbedtls.
 *
 * Original Copyright (C) 2006-2016, ARM Limited, All Rights Reserved, Apache 2.0 License.
 * Additions Copyright (C) Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD, Apache 2.0 License.
 * Additions Copyright (C) Copyright 2017 pcbreflux, Apache 2.0 License.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <string.h>
#include <stdlib.h>

#include "sdkconfig.h"

#include "MQTTClient.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_wpa2.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_system.h"
#include "lwip/apps/sntp.h"

#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"


#include "mbedtls/platform.h"
#include "mbedtls/net.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"

#include "LEDs.h"
#include "STMCommunication.h"
#include "wifi.h"

static const char *TAG = "MQTT";

bool sendingDataReady;
extern char recvbuf[129];


static void mqtt_task(void *pvParameters)
{
    uint8_t chipid[6];
    esp_efuse_mac_get_default(chipid);
    ESP_LOGI("phy", "%02x:%02x:%02x:%02x:%02x:%02x\n",chipid[0], chipid[1], chipid[2], chipid[3], chipid[4], chipid[5]);

	int ret;
	Network network;

    while(1) {
    	//getTime();
    	//Communicate();
    	//ProcessReceiveStateMachine();
        ProcessStateMachine();

    	while(receiveState != WAITALLSENT)
    	{
    		//ESP_LOGI("SPI", "receivestate %d", receiveState);
    		if (postTransmit)
			{
				postTransmit = false;
				ESP_LOGD("SPI", "PostTransmit");


			}
    		//ProcessReceiveStateMachine();
            ProcessStateMachine();
    	}

        /* Wait for the callback to set the CONNECTED_BIT in the
           event group.
        */
        /*xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
                            false, true, portMAX_DELAY);
        ESP_LOGI(TAG, "Connected to AP");

		ESP_LOGI(TAG, "Start MQTT Task ...");

		MQTTClient client;
		NetworkInit(&network);
		network.websocket = MQTT_WEBSOCKET;

		ESP_LOGI(TAG,"NetworkConnect %s:%d ...",MQTT_SERVER,MQTT_PORT);
		NetworkConnect(&network, MQTT_SERVER, MQTT_PORT);
		ESP_LOGI(TAG,"MQTTClientInit  ...");
		MQTTClientInit(&client, &network,
			2000,            // command_timeout_ms
			mqtt_sendBuf,         //sendbuf,
			MQTT_BUF_SIZE, //sendbuf_size,
			mqtt_readBuf,         //readbuf,
			MQTT_BUF_SIZE  //readbuf_size
		);

		MQTTString clientId = MQTTString_initializer;
		//clientId.cstring = "ESP32MQTTSensorboxV1_3298478923742";
		clientId.cstring = "d:nsahc1:Sensorbox-V1-Typ:Sensorbox1";

		MQTTString userName = MQTTString_initializer;
		//userName.cstring = "felixschuette";
		//userName.cstring = "lohmannd";
		userName.cstring = "use-token-auth";

		MQTTString password = MQTTString_initializer;
		//password.cstring = "c746b1b0b9084b6fabbeb5d63f7a38aa";
		//password.cstring = "ccd56c45c7554110ad93d923b7fde465";
		password.cstring = "9?_yxaV-7txcW6E6&x";

		MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
		data.clientID          = clientId;
		data.willFlag          = 0;
		data.MQTTVersion       = 4; // 3 = 3.1 4 = 3.1.1
		data.keepAliveInterval = 60;
		data.cleansession      = 1;
		//data.username = userName;
		//data.password = password;


		ESP_LOGI(TAG,"MQTTConnect  ...");
		ret = MQTTConnect(&client, &data);
		if (ret != SUCCESS) {
			ESP_LOGI(TAG, "MQTTConnect not SUCCESS: %d", ret);
			goto exit;
		}

		char msgbuf[100];

		for (int i = 0; i < 99; i++)
		{
			msgbuf[i] = recvbuf[i];
		}
		for (int i = 0; i < countBlocks; i++) {

			MQTTMessage message;
			//sprintf(msgbuf, "field1=%lf&field2=%lf&field3=%lf",(double)(values.STMTemperature),(double)(values.AkkuVoltage),(double)(values.Temperature));

			//ESP_LOGI(TAG, "MQTTPublish  ... %s",msgbuf);
			message.qos = QOS0;
			message.retained = false;
			message.dup = false;
			message.payload = (void*)(blocks[i].data);
			message.payloadlen = blocks[i].len;//strlen(msgbuf)+1;

			//ret = MQTTPublish(&client, "felixschuette/feeds/rhizostream", &message);
			//ret = MQTTPublish(&client, "iot-2/evt/SensorData/fmt/binary", &message);
			ret = MQTTPublish(&client, "rhizotech/sensorbox1/values", &message);
			if (ret != SUCCESS) {
				ESP_LOGI(TAG, "MQTTPublish not SUCCESS: %d", ret);
				goto exit;
			}
			
		}
		//gpio_set_level(GPIO_REA, 1);

		//esp_wifi_stop();
		//esp_deep_sleep_start();
		goto exit;
		exit:			
			MQTTDisconnect(&client);
			NetworkDisconnect(&network);
			vTaskDelay(1000 / portTICK_RATE_MS);
			gpio_set_level(GPIO_REA, 1);
			for(int countdown = 60; countdown >= 0; countdown--) {
				if(countdown%10==0) {
					ESP_LOGI(TAG, "%d...", countdown);
				}
				vTaskDelay(1000 / portTICK_RATE_MS);
			}
			ESP_LOGI(TAG, "Starting again!");*/
    }
 }

static void wpa2_enterprise_example_task(void *pvParameters)
{
    tcpip_adapter_ip_info_t ip;
    memset(&ip, 0, sizeof(tcpip_adapter_ip_info_t));
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    while (1) {
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        if (tcpip_adapter_get_ip_info(ESP_IF_WIFI_STA, &ip) == 0) {
            ESP_LOGI(TAG, "~~~~~~~~~~~");
            ESP_LOGI(TAG, "IP:"IPSTR, IP2STR(&ip.ip));
            ESP_LOGI(TAG, "MASK:"IPSTR, IP2STR(&ip.netmask));
            ESP_LOGI(TAG, "GW:"IPSTR, IP2STR(&ip.gw));
            ESP_LOGI(TAG, "~~~~~~~~~~~");
        }
    }
}



void app_main()
{
    nvs_flash_init();
	//esp_bluedroid_disable();
	//esp_bt_controller_disable();

    //initialise_wifi();
	ConfigureSPISlave();
	ConfigureLEDs();
	//xTaskCreate(&wpa2_enterprise_example_task, "wpa2_enterprise_example_task", 4096, NULL, 5, NULL);



    xTaskCreate(&mqtt_task, "mqtt_task", 8192, NULL, 5, NULL);
}
