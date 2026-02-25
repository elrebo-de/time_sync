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
        void setSntpServers( std::string sntpServer1,  // 1. sntp server
                                      std::string sntpServer2,  // 2. sntp server
                                      std::string sntpServer3   // 3. sntp server
                                    );
        void setTimezone(std::string timezone);
        void setSyncIntervalMs(uint32_t syncIntervalMs);

        uint32_t getSyncIntervalMs();
        bool isSynchronized();

        void initializeSntp();
        void obtainTime(void);
        void printCalendar();

    private:
        TimeSync() {}                 // Constructor

        std::string tag = "TimeSync";
        std::string sntpServer1 = "pool.ntp.org";
        std::string sntpServer2 = "time.nist.gov";
        std::string sntpServer3 = "time.google.com";

        uint32_t syncIntervalMs = 300000;

        bool timeSynchronized = false;

    public:
        TimeSync(TimeSync const&) = delete;
        void operator=(TimeSync const&) = delete;
};

#endif /* TIME_SYNC_HPP_ */
