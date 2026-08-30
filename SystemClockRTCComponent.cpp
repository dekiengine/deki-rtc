#include "SystemClockRTCComponent.h"
#include "DekiRTC.h"
#include "PackageConfig.h"
#include "DekiLogSystem.h"

static SystemClockRTC* s_SystemClockDriver = nullptr;

void SystemClockRTCComponent::Setup(SetupCallback onComplete)
{
    if (!s_SystemClockDriver)
        s_SystemClockDriver = new SystemClockRTC();

    PackageConfig cfg;
    cfg.packageId = "rtc";
    cfg.enabled  = true;

    s_SystemClockDriver->Configure(cfg);

    const bool success = s_SystemClockDriver->Initialize();
    if (success)
    {
        DekiRTC::SetCurrent(s_SystemClockDriver);
    }
    else
    {
        DEKI_LOG_ERROR("SystemClockRTCComponent: Initialize() failed");
    }

    if (onComplete) onComplete(success);
}

DEKI_REGISTER_EDITOR_AUTO_SETUP(SystemClockRTCComponent);
