#pragma once

#include "../IDekiRTC.h"
#include "PackageConfig.h"
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

    const char* GetPackageId() const override   { return "rtc"; }
    const char* GetPackageName() const override { return "System Clock RTC"; }
    void        Configure(const Deki::PackageConfig& config) override;
    bool        Initialize() override;
    void        Shutdown() override;
    void        Update(float) override {}
    Deki::PackageState GetState() const override      { return m_State; }
    const char* GetLastError() const override  { return m_LastError.c_str(); }

    DekiDateTime Now() const override;
    bool         IsHardwareConnected() const override { return true; }
    void         SetDateTime(const DekiDateTime& dt) override;

private:
    Deki::PackageState m_State = Deki::PackageState::Uninitialized;
    std::string m_LastError;
};
