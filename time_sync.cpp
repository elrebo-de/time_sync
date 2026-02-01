/*
 * time_sync.cpp
 *
 *      Author: christophoberle
 *
 * this work is licenced under the Apache 2.0 licence
 */

#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sdkconfig.h"
#include "esp_log.h"

#include "time_sync.hpp"

#include <time.h>
#include <sys/time.h>
#include <esp_sntp.h>

TimeSync& TimeSync::getInstance()
{
    static TimeSync instance; // Guaranteed to be destroyed. Instantiated on first use.
    return instance;
}

// Function to set sntp servers
void TimeSync::set_sntp_servers( std::string sntp_server_1,
                                 std::string sntp_server_2,
                                 std::string sntp_server_3
                               )
{
    ESP_LOGI(tag.c_str(), "Set SNTP srvers");
    if(sntp_server_1 != "") this->sntp_server_1 = sntp_server_1;
    if(sntp_server_2 != "") this->sntp_server_2 = sntp_server_2;
    if(sntp_server_3 != "") this->sntp_server_3 = sntp_server_3;
}

// Function to set sync_interval_ms
void TimeSync::set_sync_interval_ms(uint32_t sync_interval_ms)
 {
     ESP_LOGI(tag.c_str(), "Setting sync_interval_ms to: %lu", sync_interval_ms);
     this->sync_interval_ms = sync_interval_ms;
 }

// Function to get sync_interval_ms
uint32_t TimeSync::get_sync_interval_ms()
{
    return this->sync_interval_ms;
}

// Function to initialize SNTP  with multiple servers
void TimeSync::initialize_sntp()
{
    ESP_LOGI(tag.c_str(), "Initializing SNTP");
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);

    // Set multiple NTP servers
    esp_sntp_setservername(0, this->sntp_server_1.c_str()); // Primary server
    esp_sntp_setservername(1, this->sntp_server_2.c_str()); // Secondary server
    esp_sntp_setservername(2, this->sntp_server_3.c_str()); // Tertiary server

    esp_sntp_init();
}

// Function to obtain and print time
void TimeSync::obtain_time(void)
{
    // Wait for time to be set
    time_t now = 0;
    struct tm timeinfo = {0};
    int retry = 0;
    const int retry_count = 10;

    while (timeinfo.tm_year < (2025 - 1900) && ++retry < retry_count)
    {
        ESP_LOGD(tag.c_str(), "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (retry == retry_count)
    {
        ESP_LOGE(tag.c_str(), "Failed to obtain time.");
        return;
    }

    ESP_LOGI(tag.c_str(), "Time synchronized.");
}

// Function to set timezone
void TimeSync::set_timezone(std::string timezone)
{
    ESP_LOGI(tag.c_str(), "Setting timezone to: %s", timezone.c_str());
    setenv("TZ", timezone.c_str(), 1); // Set the TZ environment variable
    tzset(); // Apply the timezone change
}

// Print current calendar information
void TimeSync::print_calendar()
{
    time_t now;
    struct tm timeinfo;
    char buffer[64];

    time(&now);
    localtime_r(&now, &timeinfo);

    strftime(buffer, sizeof(buffer), "Date: %Y-%m-%d, Time: %H:%M:%S", &timeinfo);
    ESP_LOGI(tag.c_str(), "%s", buffer);
    ESP_LOGI(tag.c_str(), "Weekday: %d (0=Sunday, 6=Saturday)", timeinfo.tm_wday);
    ESP_LOGI(tag.c_str(), "Day of the year: %d", timeinfo.tm_yday + 1);
}
