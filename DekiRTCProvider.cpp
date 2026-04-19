#include "DekiRTCProvider.h"
#include "DekiLogSystem.h"

DekiRTCProvider::Factory DekiRTCProvider::s_Factory = nullptr;
IDekiRTC*                DekiRTCProvider::s_Current = nullptr;

void DekiRTCProvider::SetFactory(Factory factory)
{
    s_Factory = factory;
    DEKI_LOG_INTERNAL("DekiRTCProvider: Factory registered");
}

IDekiRTC* DekiRTCProvider::Create()
{
    if (!s_Factory)
    {
        DEKI_LOG_ERROR("DekiRTCProvider: No factory registered - module must call SetFactory()");
        return nullptr;
    }
    return s_Factory();
}

bool DekiRTCProvider::HasFactory()
{
    return s_Factory != nullptr;
}

void DekiRTCProvider::SetCurrent(IDekiRTC* rtc)
{
    s_Current = rtc;
}

IDekiRTC* DekiRTCProvider::GetCurrent()
{
    return s_Current;
}
