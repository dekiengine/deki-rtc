#pragma once

#include <cstdint>
#include <deki/SetupComponent.h>
#include <deki/reflection/Property.h>
#include "chips/DS3231RTC.h"

namespace DekiRtc
{

/**
 * @brief Boot-scene component for the DS3231 real-time clock.
 *
 * Wires to the shared I2C bus at address 0x68. Requires an I2CBusComponent on
 * the matching port in boot.scene.
 */
DEKI_CATEGORY("Sensors")
DEKI_DISPLAY_NAME("DS3231 RTC")
DEKI_DESCRIPTION("Reads and sets the DS3231 real-time clock over I2C.")
DEKI_FORMER_NAME("DS3231RTCComponent")
class DS3231RTCComponent : public Deki::SetupComponent
{
public:

    /** @brief Which I2C bus this chip is wired to. */
    DEKI_EXPORT
    DEKI_TOOLTIP("Which I2C bus the clock chip is on. Must match the I2C Bus component that set that port up.")
    DEKI_RANGE(0, 3)
    int32_t i2cPort = 0;

    DS3231RTCComponent() = default;
    virtual ~DS3231RTCComponent() = default;

    void        Setup(SetupCallback onComplete) override;
    const char* GetSetupName() const override { return "DS3231 RTC"; }
};

}  // namespace DekiRtc

