#pragma once

#include "IDekiRTC.h"

namespace DekiRtc
{

/// Holds the active real-time clock driver.
///
/// A SetupComponent (DS3231RTCComponent on a device, SystemClockRTCComponent
/// on desktop and in the editor) registers its driver with SetCurrent()
/// during Setup(). Game and editor code read the time through
/// GetCurrent()->Now().
class DekiRTC
{
public:
    static void SetCurrent(IDekiRTC* rtc);
    static IDekiRTC* GetCurrent();

private:
    static IDekiRTC* s_Current;
};

}  // namespace DekiRtc
