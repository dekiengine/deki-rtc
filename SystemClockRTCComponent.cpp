#include "SystemClockRTCComponent.h"
#include "DekiRTC.h"
#include "ModuleConfig.h"
#include "DekiLogSystem.h"

static SystemClockRTC* s_Driver = nullptr;

void SystemClockRTCComponent::Setup(SetupCallback onComplete)
{
    if (!s_Driver)
        s_Driver = new SystemClockRTC();

    ModuleConfig cfg;
    cfg.moduleId = "rtc";
    cfg.enabled  = true;

    s_Driver->Configure(cfg);

    const bool success = s_Driver->Initialize();
    if (success)
    {
        DekiRTC::SetCurrent(s_Driver);
    }
    else
    {
        DEKI_LOG_ERROR("SystemClockRTCComponent: Initialize() failed");
    }

    if (onComplete) onComplete(success);
}

DEKI_REGISTER_EDITOR_AUTO_SETUP(SystemClockRTCComponent);
