#pragma once

#include "SetupComponent.h"
#include "reflection/DekiProperty.h"
#include "chips/SystemClockRTC.h"

/**
 * @brief Editor / desktop SetupComponent that registers a SystemClockRTC with
 * DekiRTC. Mirrors DS3231RTCComponent (embedded) — same SetupComponent
 * shape, just no I2C config because the host OS clock has no hardware deps.
 *
 * Auto-fired by SetupComponent::RunEditorAutoSetups() after package load.
 */
class SystemClockRTCComponent : public SetupComponent
{
public:
    DEKI_COMPONENT(SystemClockRTCComponent, SetupComponent, "System", "2c7d4e83-1f5a-4b29-9e6d-3c8b0f1a4d72", "")
    DEKI_DISPLAY_NAME("System Clock RTC")
    DEKI_DESCRIPTION("Uses the computer's own clock as the real-time clock, for editor and desktop runs.")

    SystemClockRTCComponent() = default;
    virtual ~SystemClockRTCComponent() = default;

    void        Setup(SetupCallback onComplete) override;
    const char* GetSetupName() const override { return "System Clock RTC"; }
};

#include "generated/SystemClockRTCComponent.gen.h"
