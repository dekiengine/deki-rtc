#include "DS3231RTCComponent.h"
#include "DekiRTCProvider.h"
#include "modules/ModuleConfig.h"
#include "DekiLogSystem.h"
#include <string>

static DS3231RTC* s_Driver = nullptr;

void DS3231RTCComponent::Setup(SetupCallback onComplete)
{
    if (!s_Driver)
        s_Driver = new DS3231RTC();

    ModuleConfig cfg;
    cfg.moduleId = "rtc";
    cfg.enabled  = true;
    cfg.settings["i2c_port"] = std::to_string(i2c_port);

    s_Driver->Configure(cfg);

    const bool success = s_Driver->Initialize();
    if (success)
    {
        DekiRTCProvider::SetCurrent(s_Driver);
    }
    else
    {
        DEKI_LOG_ERROR("DS3231RTCComponent: Failed to initialize DS3231 on I2C port %d", (int)i2c_port);
    }

    if (onComplete) onComplete(success);
}
