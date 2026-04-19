#include "RTCComponent.h"
#include "DekiRTCProvider.h"
#include "modules/ModuleConfig.h"
#include "DekiLogSystem.h"
#include <string>

static IDekiRTC* s_RTCModule = nullptr;

void RTCComponent::Setup(SetupCallback onComplete)
{
    if (!s_RTCModule)
    {
        s_RTCModule = DekiRTCProvider::Create();
        if (!s_RTCModule)
        {
            DEKI_LOG_ERROR("RTCComponent: No RTC backend registered");
            if (onComplete) onComplete(false);
            return;
        }
    }

    ModuleConfig config;
    config.moduleId = "rtc";
    config.enabled  = true;
    config.pins["SDA"] = sda_pin;
    config.pins["SCL"] = scl_pin;
    config.settings["i2c_hz"] = std::to_string(i2c_hz);

    s_RTCModule->Configure(config);

    bool success = s_RTCModule->Initialize();
    if (success)
    {
        DekiRTCProvider::SetCurrent(s_RTCModule);
    }
    else
    {
        DEKI_LOG_ERROR("RTCComponent: Failed to initialize RTC backend");
    }

    if (onComplete) onComplete(success);
}

IDekiRTC* RTCComponent::GetRTCModule()
{
    return s_RTCModule;
}
