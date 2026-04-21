#pragma once

#include "../IDekiRTC.h"
#include "modules/ModuleConfig.h"
#include "providers/IDekiI2C.h"
#include <string>

class DS3231RTC : public IDekiRTC
{
public:
    DS3231RTC() = default;
    ~DS3231RTC() override = default;

    const char* GetModuleId() const override   { return "rtc"; }
    const char* GetModuleName() const override { return "DS3231 RTC (I\xC2\xB2""C)"; }
    void        Configure(const ModuleConfig& config) override;
    bool        Initialize() override;
    void        Shutdown() override;
    void        Update(float) override {}
    ModuleState GetState() const override      { return m_State; }
    const char* GetLastError() const override  { return m_LastError.c_str(); }

    DekiDateTime Now() const override;
    bool         IsHardwareConnected() const override { return m_HardwareConnected; }
    void         SetDateTime(const DekiDateTime& dt) override;

private:
    int          m_BusPort = 0;
    IDekiI2C*    m_Bus     = nullptr;
    static constexpr uint8_t kI2cAddr = 0x68;

    ModuleState  m_State = ModuleState::Uninitialized;
    bool         m_HardwareConnected = false;
    std::string  m_LastError;
};
