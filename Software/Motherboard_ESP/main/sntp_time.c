/*
 * sntp_time.c
 *
 *  Created on: Okt 5, 2018
 *      Author: Daniel Lohmann
 */
#include "sntp_time.h"
#include "wifi.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "lwip/apps/sntp.h"


static const char *TAG_SNTP = "SNTP";

static void initialize_sntp(char* timeServer)
{
    ESP_LOGI(TAG_SNTP, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    ESP_LOGI(TAG_SNTP, "Time server %s", timeServer);
    sntp_setservername(0, timeServer);
    sntp_init();
}

static void obtain_time(char* timeServer, bool* synced)
{
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
                        false, true, portMAX_DELAY);
    initialize_sntp(timeServer);

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;
    while(timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
        ESP_LOGI(TAG_SNTP, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }
    *synced = timeinfo.tm_year >= (2016 - 1900);
}

time_t now;

time_t GetTime(char* timeServer, bool* synced)
{
	struct tm timeinfo;
	char strftime_buf[64];
	ESP_LOGI(TAG_SNTP, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
	obtain_time(timeServer, synced);
	// update 'now' variable with current time
	time(&now);

	localtime_r(&now, &timeinfo);
	strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
	ESP_LOGI(TAG_SNTP, "The current date/time in UTC is: %s", strftime_buf);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    return now;
}