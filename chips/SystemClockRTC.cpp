#include "SystemClockRTC.h"
#include <chrono>
#include <ctime>

void SystemClockRTC::Configure(const Deki::PackageConfig&)
{
}

bool SystemClockRTC::Initialize()
{
    m_State = Deki::PackageState::Initialized;
    return true;
}

void SystemClockRTC::Shutdown()
{
    m_State = Deki::PackageState::Uninitialized;
}

DekiDateTime SystemClockRTC::Now() const
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &t);
#else
    localtime_r(&t, &local);
#endif

    DekiDateTime dt{};
    dt.year   = static_cast<uint16_t>(local.tm_year + 1900);
    dt.month  = static_cast<uint8_t>(local.tm_mon + 1);
    dt.day    = static_cast<uint8_t>(local.tm_mday);
    dt.hour   = static_cast<uint8_t>(local.tm_hour);
    dt.minute = static_cast<uint8_t>(local.tm_min);
    dt.second = static_cast<uint8_t>(local.tm_sec);
    return dt;
}

void SystemClockRTC::SetDateTime(const DekiDateTime&)
{
    m_LastError = "SystemClockRTC: setting the host clock is not supported";
}
