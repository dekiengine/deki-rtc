#pragma once

#include "IDekiRTC.h"

/**
 * @brief Active-driver registry and one-call facade for RTC.
 *
 * A SetupComponent (DS3231RTCComponent on embedded, SystemClockRTCComponent
 * on desktop / editor) registers its IDekiRTC driver via SetCurrent() during
 * Setup(). Game / editor code reads the current time via Now() — handles the
 * provider lookup and null check internally and is the single entry point.
 * GetCurrent() is exposed for cases where you need the driver pointer
 * directly (e.g. SetDateTime).
 */
class DekiRTC
{
public:
    static void      SetCurrent(IDekiRTC* rtc);
    static IDekiRTC* GetCurrent();

private:
    static IDekiRTC* s_Current;
};
