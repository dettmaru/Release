/*
 * wifi.h
 *
 *  Created on: Okt 5, 2018
 *      Author: Daniel Lohmann
 */
#include "wifi.h"
#include <stdlib.h>

#include "esp_wifi.h"
#include "esp_wpa2.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include "esp_system.h"
#include <string.h>

const int CONNECTED_BIT = BIT0;
char* TAG = "wifi";

char* CONF_WLANSSID = NULL;
char* CONF_WLANPassword = NULL;


/* FreeRTOS event group to signal when we are connected & ready to make a request */
EventGroupHandle_t wifi_event_group;

#ifdef WPA2_PERSONAL
static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently
           auto-reassociate. */
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}
bool initialized = false;
void initialise_wifi(void)
{
    if (!initialized)
    {
        initialized = true;
        tcpip_adapter_init();
        wifi_event_group = xEventGroupCreate();
        ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
        ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );

        ESP_LOGI("WIFI", "WLAN-SSID: %s", CONF_WLANSSID);
        ESP_LOGI("WIFI", "WLAN-Password: %s", CONF_WLANPassword);

        wifi_config_t wifi_config = { };
        strcpy((char*)wifi_config.sta.ssid, CONF_WLANSSID);
        strcpy((char*)wifi_config.sta.password, CONF_WLANPassword);

        ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
        ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
        ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
        ESP_ERROR_CHECK( esp_wifi_start() );
    }
}
#endif

#ifdef WPA2_ENTERPRISE
/* CA cert, taken from wpa2_ca.pem
   Client cert, taken from wpa2_client.crt
   Client key, taken from wpa2_client.key

   The PEM, CRT and KEY file were provided by the person or organization
   who configured the AP with wpa2 enterprise.

   To embed it in the app binary, the PEM, CRT and KEY file is named
   in the component.mk COMPONENT_EMBED_TXTFILES variable.
*/
extern uint8_t ca_pem_start[] asm("_binary_wpa2_ca_pem_start");
extern uint8_t ca_pem_end[]   asm("_binary_wpa2_ca_pem_end");
extern uint8_t client_crt_start[] asm("_binary_wpa2_client_crt_start");
extern uint8_t client_crt_end[]   asm("_binary_wpa2_client_crt_end");
extern uint8_t client_key_start[] asm("_binary_wpa2_client_key_start");
extern uint8_t client_key_end[]   asm("_binary_wpa2_client_key_end");

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

static void initialise_wifi(void)
{
    unsigned int ca_pem_bytes = ca_pem_end - ca_pem_start;
    unsigned int client_crt_bytes = client_crt_end - client_crt_start;
    unsigned int client_key_bytes = client_key_end - client_key_start;
    esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();

    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_WIFI_SSID,
        },
    };
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_sta_wpa2_ent_set_ca_cert(ca_pem_start, ca_pem_bytes) );
    ESP_ERROR_CHECK( esp_wifi_sta_wpa2_ent_set_cert_key(client_crt_start, client_crt_bytes,\
    		client_key_start, client_key_bytes, NULL, 0) );

    if (strlen(EXAMPLE_WIFI_EAP_ID) > 0)
    {
    	ESP_ERROR_CHECK( esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EXAMPLE_WIFI_EAP_ID, strlen(EXAMPLE_WIFI_EAP_ID)) );
    }
    if (EXAMPLE_WIFI_EAP_METHOD == EAP_PEAP || EXAMPLE_WIFI_EAP_METHOD == EAP_TTLS) {
        ESP_ERROR_CHECK( esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EXAMPLE_WIFI_EAP_USERNAME, strlen(EXAMPLE_WIFI_EAP_USERNAME)) );
        ESP_ERROR_CHECK( esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EXAMPLE_WIFI_EAP_PASSWORD, strlen(EXAMPLE_WIFI_EAP_PASSWORD)) );
    }

    ESP_ERROR_CHECK( esp_wifi_sta_wpa2_ent_enable(&config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}
#endif