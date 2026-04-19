#pragma once

#include "providers/IDekiModule.h"
#include <cstdint>

struct DekiDateTime
{
    uint16_t year   = 0;
    uint8_t  month  = 0;
    uint8_t  day    = 0;
    uint8_t  hour   = 0;
    uint8_t  minute = 0;
    uint8_t  second = 0;
};

class IDekiRTC : public IDekiModule
{
public:
    const char* GetModuleCategory() const override { return "rtc"; }

    virtual DekiDateTime Now() const = 0;
    virtual bool         IsHardwareConnected() const = 0;

    virtual void SetDateTime(const DekiDateTime& dt) = 0;
};
