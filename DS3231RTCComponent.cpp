#include "DS3231RTCComponent.h"
#include "DekiRTC.h"
#include "PackageConfig.h"
#include "DekiLogSystem.h"
#include <string>

static DS3231RTC* s_DS3231Driver = nullptr;

void DS3231RTCComponent::Setup(SetupCallback onComplete)
{
    if (!s_DS3231Driver)
        s_DS3231Driver = new DS3231RTC();

    Deki::PackageConfig cfg;
    cfg.packageId = "rtc";
    cfg.enabled  = true;
    cfg.settings["i2cPort"] = std::to_string(i2cPort);

    s_DS3231Driver->Configure(cfg);

    const bool success = s_DS3231Driver->Initialize();
    if (success)
    {
        DekiRTC::SetCurrent(s_DS3231Driver);
    }
    else
    {
        DEKI_LOG_ERROR("DS3231RTCComponent: Failed to initialize DS3231 on I2C port %d", (int)i2cPort);
    }

    if (onComplete) onComplete(success);
}
