#pragma once

#include <cstdint>
#include "SetupComponent.h"
#include "reflection/DekiProperty.h"
#include "IDekiRTC.h"

class RTCComponent : public SetupComponent
{
public:
    DEKI_COMPONENT(RTCComponent, SetupComponent, "System", "c4a8b3d1-7e92-4f56-8b1c-3d9e2f4a5b67", "")

    DEKI_EXPORT
    DEKI_RANGE(0, 48)
    int32_t sda_pin = 21;

    DEKI_EXPORT
    DEKI_RANGE(0, 48)
    int32_t scl_pin = 22;

    DEKI_EXPORT
    DEKI_RANGE(10000, 1000000)
    int32_t i2c_hz = 400000;

    RTCComponent() = default;
    virtual ~RTCComponent() = default;

    void        Setup(SetupCallback onComplete) override;
    const char* GetSetupName() const override { return "RTC"; }

    static IDekiRTC* GetRTCModule();
};

#include "generated/RTCComponent.gen.h"
