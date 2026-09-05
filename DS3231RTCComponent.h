#pragma once

#include <cstdint>
#include "SetupComponent.h"
#include "reflection/DekiProperty.h"
#include "chips/DS3231RTC.h"

/**
 * @brief Boot-scene component for the DS3231 real-time clock.
 *
 * Wires to the shared I2C bus at address 0x68. Requires an I2CBusComponent on
 * the matching port in boot.scene.
 */
class DS3231RTCComponent : public Deki::SetupComponent
{
public:
    DEKI_COMPONENT(DS3231RTCComponent, Deki::SetupComponent, "Sensors", "3e9b172c-4a5d-4c08-b5e7-91f3a0c2d74b", "")
    DEKI_DISPLAY_NAME("DS3231 RTC")
    DEKI_DESCRIPTION("Reads and sets the DS3231 real-time clock over I2C.")

    /** @brief Which I2C bus this chip is wired to. */
    DEKI_EXPORT
    DEKI_RANGE(0, 3)
    int32_t i2cPort = 0;

    DS3231RTCComponent() = default;
    virtual ~DS3231RTCComponent() = default;

    void        Setup(SetupCallback onComplete) override;
    const char* GetSetupName() const override { return "DS3231 RTC"; }
};

#include "generated/DS3231RTCComponent.gen.h"
