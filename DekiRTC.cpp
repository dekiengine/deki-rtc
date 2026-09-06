#include "DekiRTC.h"

IDekiRTC* DekiRTC::s_Current = nullptr;

void DekiRTC::SetCurrent(IDekiRTC* rtc)
{
    s_Current = rtc;
}

IDekiRTC* DekiRTC::GetCurrent()
{
    return s_Current;
}
