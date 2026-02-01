/*
 * Example program to use time_sync functionality with elrebo-de/time_sync
 */

#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "wifi_manager.hpp"
#include "time_sync.hpp"

static const char *tag = "time sync test";

static void timeTask(void *pc){
    TimeSync* timeSync = &TimeSync::getInstance();

    // Synchronize time
    timeSync->obtain_time();

    while (1)
    {
        timeSync->print_calendar();
        vTaskDelay(pdMS_TO_TICKS(timeSync->get_sync_interval_ms())); // Print calendar every 5 minutes
    }
}

extern "C" void app_main(void)
{
    // short delay to reconnect logging
    vTaskDelay(pdMS_TO_TICKS(500)); // delay 0.5 seconds

    ESP_LOGI(tag, "Example Program");

    /* Initialize Wifi */
    Wifi wifi("Wifi",
              "ESP32",
              "de-DE");

    /* Initialize TimeSync class */
    ESP_LOGI(tag, "TimeSync");
    TimeSync* timeSync = &timeSync->getInstance();
    timeSync->initialize_sntp();
    timeSync->set_timezone(std::string("CET"));
    timeSync->set_sync_interval_ms(30000);

    xTaskCreate(timeTask, "time_task", 4096, NULL, 5, NULL);

    while(1) {
        ESP_LOGI(tag, "sleep 10 seconds");
        vTaskDelay(pdMS_TO_TICKS(10000)); // delay 10 seconds
    }
}
