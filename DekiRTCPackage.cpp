// Package entry point for deki-rtc.
#include "DekiRTCPackage.h"
#include <deki/interop/Plugin.h>
#include <deki/LogSystem.h>
#include "DekiRTC.h"

extern void DekiRTCRegisterComponents();
extern int DekiRTCGetAutoComponentCount();
extern const Deki::ComponentMeta* DekiRTCGetAutoComponentMeta(int index);

namespace DekiRtc
{

#ifdef DEKI_EDITOR

static bool s_RTCRegistered = false;

// The exports below are C symbols at global scope; the package's own
// registration helpers and statics live in its namespace.
using namespace DekiRtc;

extern "C"
{
    DEKI_RTC_API int DekiRTCEnsureRegistered(void)
    {
        if (s_RTCRegistered)
        {
            return ::DekiRTCGetAutoComponentCount();
        }
        s_RTCRegistered = true;
        ::DekiRTCRegisterComponents();
        return ::DekiRTCGetAutoComponentCount();
    }

    DEKI_PLUGIN_API const char* DekiPluginGetName(void)
    {
        return "Deki RTC Package";
    }
    DEKI_PLUGIN_API const char* DekiPluginGetVersion(void)
    {
#ifdef DEKI_PACKAGE_VERSION
        return DEKI_PACKAGE_VERSION;
#else
        return "0.0.0-dev";
#endif
    }
    DEKI_PLUGIN_API int DekiPluginInit(void)
    {
        return 0;
    }
    DEKI_PLUGIN_API void DekiPluginShutdown(void)
    {
        s_RTCRegistered = false;
        // Clear the provider so a hot reload leaves no pointer to a driver
        // whose code unloads with the DLL. The driver object itself is leaked
        // on purpose: as on a device, a driver lives until the process exits.
        DekiRTC::SetCurrent(nullptr);
    }
    DEKI_PLUGIN_API int DekiPluginGetComponentCount(void)
    {
        return ::DekiRTCGetAutoComponentCount();
    }
    DEKI_PLUGIN_API const Deki::ComponentMeta* DekiPluginGetComponentMeta(int index)
    {
        return ::DekiRTCGetAutoComponentMeta(index);
    }
    DEKI_PLUGIN_API void DekiPluginRegisterComponents(void)
    {
        DekiRTCEnsureRegistered();
    }

}  // extern "C"

#endif  // DEKI_EDITOR
}  // namespace DekiRtc
