/*
 * wifi.h
 *
 *  Created on: Okt 5, 2018
 *      Author: Daniel Lohmann
 */
#ifndef WIFI_H
#define WIFI_H

#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define WPA2_PERSONAL
//#define WPA2_ENTERPRISE

#ifdef WPA2_PERSONAL
/* The examples use simple WiFi configuration that you can set via
   'make menuconfig'.

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_WIFI_SSID CONFIG_WIFI_SSID
#define EXAMPLE_WIFI_PASS CONFIG_WIFI_PASSWORD

#endif

#ifdef WPA2_ENTERPRISE
#define EXAMPLE_WIFI_SSID CONFIG_WIFI_SSID
#define EXAMPLE_WIFI_EAP_METHOD CONFIG_EAP_METHOD
#define EXAMPLE_WIFI_EAP_ID CONFIG_EAP_ID
#define EXAMPLE_WIFI_EAP_USERNAME CONFIG_EAP_USERNAME
#define EXAMPLE_WIFI_EAP_PASSWORD CONFIG_EAP_PASSWORD
#endif
/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
extern const int CONNECTED_BIT;

/* Constants that aren't configurable in menuconfig */
#define EAP_PEAP 1
#define EAP_TTLS 2

extern char* CONF_WLANSSID;
extern char* CONF_WLANPassword;

extern EventGroupHandle_t wifi_event_group;

void initialise_wifi(void);

#endif // WIFI_H