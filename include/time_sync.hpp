/*
 * time_sync.hpp
 *
 *      Author: christophoberle
 *
 * this work is licenced under the MIT licence
 */

#ifndef TIME_SYNC_HPP_
#define TIME_SYNC_HPP_

#include <string>

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

    public:
        TimeSync(TimeSync const&) = delete;
        void operator=(TimeSync const&) = delete;
};

#endif /* TIME_SYNC_HPP_ */
