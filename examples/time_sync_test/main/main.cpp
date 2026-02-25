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
    timeSync->obtainTime();

    while (1)
    {
        timeSync->printCalendar();
        vTaskDelay(pdMS_TO_TICKS(timeSync->getSyncIntervalMs())); // Print calendar every sync interval
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
    timeSync->initializeSntp();
    timeSync->setTimezone(std::string("CET"));
    timeSync->setSyncIntervalMs(30000); // syncIntervalMs set to 30 sec, default for normal operation is 5 mins

    xTaskCreate(timeTask, "timeTask", 4096, NULL, 5, NULL);

    while(!timeSync->isSynchronized()) {
        ESP_LOGI(tag, "time is not yet synchronized");
        vTaskDelay(pdMS_TO_TICKS(1000)); // delay 1 second
    }

    while(1) {
        ESP_LOGI(tag, "wait 10 seconds");
        vTaskDelay(pdMS_TO_TICKS(10000)); // delay 10 seconds
    }
}
