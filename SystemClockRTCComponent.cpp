#include "SystemClockRTCComponent.h"
#include "DekiRTC.h"
#include <deki/PackageConfig.h>
#include <deki/LogSystem.h>

namespace DekiRtc
{

static SystemClockRTC* s_SystemClockDriver = nullptr;

void SystemClockRTCComponent::Setup(SetupCallback onComplete)
{
    if (!s_SystemClockDriver)
        s_SystemClockDriver = new SystemClockRTC();

    Deki::PackageConfig cfg;
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

// Project open, not Play. Reading the machine's clock is local and free, and
// a component that shows a time should show one in the editor too.
DEKI_REGISTER_EDITOR_AUTO_SETUP(SystemClockRTCComponent);

}  // namespace DekiRtc
