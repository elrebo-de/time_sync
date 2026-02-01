idf_component_register( SRCS "time_sync.cpp"
                        INCLUDE_DIRS "include"
                        REQUIRES "driver"
                                 "esp_timer"
                                 "esp_event"
                      )

