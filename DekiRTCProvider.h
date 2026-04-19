#pragma once

#include "IDekiRTC.h"

class DekiRTCProvider
{
public:
    using Factory = IDekiRTC* (*)();

    static void     SetFactory(Factory factory);
    static IDekiRTC* Create();
    static bool     HasFactory();

    static void      SetCurrent(IDekiRTC* rtc);
    static IDekiRTC* GetCurrent();

private:
    static Factory   s_Factory;
    static IDekiRTC* s_Current;
};
