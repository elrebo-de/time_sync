# Time Synchronization component

A simple Time Synchronization as an ESP-IDF component. It runs on any ESP32 processor and is built using the ESP-IDF build system in version 5.5+.

The component is implemented as C++ class `TimeSync`.

## Connecting the component and Usage

You need to include ```time_sync.hpp``` and 

provide a function ```timeTask``` for the task which synchronizes the time periodically.

After establishing a Wifi connection the ```TimeSync``` class must be initialized and

the timeTask must be started.

From then on the time will be synchronized.
```
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "time_sync.hpp"

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
    ... Wifi must be established here
    
    /* Initialize TimeSync class */
    ESP_LOGI(tag, "TimeSync");
    TimeSync* timeSync = &timeSync->getInstance();
    timeSync->initialize_sntp();
    timeSync->set_timezone(std::string("CET"));
    timeSync->set_sync_interval_ms(30000);

    xTaskCreate(timeTask, "time_task", 4096, NULL, 5, NULL);

    while(!timeSync->is_synchronized()) {
        ESP_LOGI(tag, "time is not yet synchronized");
        vTaskDelay(pdMS_TO_TICKS(1000)); // delay 1 second
    }

    ... now the time is synchronized and will be synchronized again every sync_interval_ms milliseconds
    
    ... do your work here
}
```

## API
The API of the component is located in the include directory ```include/time_sync.hpp``` and defines the
C++ class ```TimeSync```. 

```TimeSync``` is a Singleton class.

```C++
/* class TimeSync
   Class to synchronize time with SNTP servers.

   The original code is taken from the GitHub repository https://github.com/sachin42/idfcomponents.git
   from file HTTPClient/main.cpp.

   Source of Singleton class structure - https://stackoverflow.com/a/1008289
   Posted by Loki Astari, modified by community. See post 'Timeline' for change history
   Retrieved 2026-02-01, License - CC BY-SA 4.0
*/

class TimeSync
{
    public:
        static TimeSync& getInstance();
        void set_sntp_servers( std::string sntp_server_1,  // 1. sntp server
                                      std::string sntp_server_2,  // 2. sntp server
                                      std::string sntp_server_3   // 3. sntp server
                                    );
        void set_timezone(std::string timezone);
        void set_sync_interval_ms(uint32_t sync_interval_ms);

        uint32_t get_sync_interval_ms();
        bool is_synchronized();

        void initialize_sntp();
        void obtain_time(void);
        void print_calendar();
        void timeTask(void *pc);

    private:
        TimeSync() {}                 // Constructor

        std::string tag = "TimeSync";
        std::string sntp_server_1 = "pool.ntp.org";
        std::string sntp_server_2 = "time.nist.gov";
        std::string sntp_server_3 = "time.google.com";

        uint32_t sync_interval_ms = 300000;

        bool time_synchronized = false;

    public:
        TimeSync(TimeSync const&) = delete;
        void operator=(TimeSync const&) = delete;
};
```

# License
This component is provided under the MIT license.
