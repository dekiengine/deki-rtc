#pragma once

#include "../IDekiRTC.h"
#include "ModuleConfig.h"
#include <string>

/**
 * RTC driver backed by the host operating system clock. Used on desktop
 * builds and inside the editor where no battery-backed RTC chip is present.
 */
class SystemClockRTC : public IDekiRTC
{
public:
    SystemClockRTC() = default;
    ~SystemClockRTC() override = default;

    const char* GetModuleId() const override   { return "rtc"; }
    const char* GetModuleName() const override { return "System Clock RTC"; }
    void        Configure(const ModuleConfig& config) override;
    bool        Initialize() override;
    void        Shutdown() override;
    void        Update(float) override {}
    ModuleState GetState() const override      { return m_State; }
    const char* GetLastError() const override  { return m_LastError.c_str(); }

    DekiDateTime Now() const override;
    bool         IsHardwareConnected() const override { return true; }
    void         SetDateTime(const DekiDateTime& dt) override;

private:
    ModuleState m_State = ModuleState::Uninitialized;
    std::string m_LastError;
};
