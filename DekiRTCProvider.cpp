#include "DekiRTCProvider.h"

IDekiRTC* DekiRTCProvider::s_Current = nullptr;

void DekiRTCProvider::SetCurrent(IDekiRTC* rtc)
{
    s_Current = rtc;
}

IDekiRTC* DekiRTCProvider::GetCurrent()
{
    return s_Current;
}
