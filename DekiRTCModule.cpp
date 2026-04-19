#include "DekiRTCModule.h"
#include "interop/DekiPlugin.h"
#include "DekiRTCProvider.h"
#include "chips/DS3231RTC.h"

namespace
{
struct DekiRTCBackendInit
{
    DekiRTCBackendInit()
    {
        DekiRTCProvider::SetFactory([]() -> IDekiRTC* { return new DS3231RTC(); });
    }
};
static DekiRTCBackendInit s_rtc_init;
}

#ifdef DEKI_EDITOR

extern void DekiRTC_RegisterComponents();
extern int  DekiRTC_GetAutoComponentCount();
extern const DekiComponentMeta* DekiRTC_GetAutoComponentMeta(int index);

static bool s_RTCRegistered = false;

extern "C" {

DEKI_RTC_API int DekiRTC_EnsureRegistered(void)
{
    if (s_RTCRegistered)
        return DekiRTC_GetAutoComponentCount();
    s_RTCRegistered = true;
    DekiRTC_RegisterComponents();
    return DekiRTC_GetAutoComponentCount();
}

DEKI_PLUGIN_API const char* DekiPlugin_GetName(void)    { return "Deki RTC Module"; }
DEKI_PLUGIN_API const char* DekiPlugin_GetVersion(void)
{
#ifdef DEKI_MODULE_VERSION
    return DEKI_MODULE_VERSION;
#else
    return "0.0.0-dev";
#endif
}
DEKI_PLUGIN_API const char* DekiPlugin_GetReflectionJson(void) { return "{}"; }
DEKI_PLUGIN_API int  DekiPlugin_Init(void)             { return 0; }
DEKI_PLUGIN_API void DekiPlugin_Shutdown(void)         { s_RTCRegistered = false; }
DEKI_PLUGIN_API int  DekiPlugin_GetComponentCount(void){ return DekiRTC_GetAutoComponentCount(); }
DEKI_PLUGIN_API const DekiComponentMeta* DekiPlugin_GetComponentMeta(int index)
{
    return DekiRTC_GetAutoComponentMeta(index);
}
DEKI_PLUGIN_API void DekiPlugin_RegisterComponents(void) { DekiRTC_EnsureRegistered(); }

DEKI_PLUGIN_API int DekiPlugin_GetFeatureCount(void) { return 0; }
DEKI_PLUGIN_API const struct DekiModuleFeatureInfo* DekiPlugin_GetFeature(int) { return nullptr; }

} // extern "C"

#endif // DEKI_EDITOR
