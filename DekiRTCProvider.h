#pragma once

#include "IDekiRTC.h"

/**
 * @brief Published-instance registry for the currently active RTC driver.
 *
 * Chip-specific SetupComponents (e.g., DS3231Component) call SetCurrent() in
 * their Setup() once the driver is configured and initialized. Game code reads
 * the current RTC via GetCurrent().
 */
class DekiRTCProvider
{
public:
    static void      SetCurrent(IDekiRTC* rtc);
    static IDekiRTC* GetCurrent();

private:
    static IDekiRTC* s_Current;
};
