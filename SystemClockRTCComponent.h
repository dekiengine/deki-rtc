#pragma once

#include <deki/SetupComponent.h>
#include <deki/reflection/Property.h>
#include "chips/SystemClockRTC.h"

namespace DekiRtc
{

/// Desktop and editor counterpart of DS3231RTCComponent: registers a
/// SystemClockRTC with DekiRTC. It has no I2C settings because it reads the
/// operating system's clock.
///
/// Run automatically when a project opens.
DEKI_CATEGORY("System")
DEKI_DISPLAY_NAME("System Clock RTC")
DEKI_DESCRIPTION("Uses the computer's own clock as the real-time clock, for editor and desktop runs.")
DEKI_FORMER_NAME("SystemClockRTCComponent")
class SystemClockRTCComponent : public Deki::SetupComponent
{
public:
    SystemClockRTCComponent() = default;
    virtual ~SystemClockRTCComponent() = default;

    void Setup(SetupCallback onComplete) override;
    const char* GetSetupName() const override { return "System Clock RTC"; }
};

}  // namespace DekiRtc
