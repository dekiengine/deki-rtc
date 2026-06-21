#include "DS3231RTC.h"
#include "DekiI2C.h"  // from deki-i2c
#include "DekiLogSystem.h"

namespace
{
    inline uint8_t BcdToBin(uint8_t v) { return (uint8_t)((v >> 4) * 10 + (v & 0x0F)); }
    inline uint8_t BinToBcd(uint8_t v) { return (uint8_t)(((v / 10) << 4) | (v % 10)); }
}

void DS3231RTC::Configure(const ModuleConfig& config)
{
    m_BusPort = config.GetInt("i2c_port", 0);
}

bool DS3231RTC::Initialize()
{
    m_Bus = DekiI2C::GetBus(m_BusPort);
    if (!m_Bus)
    {
        m_LastError = "DS3231: no I2C bus registered on requested port (add an I2CBusComponent)";
        m_State = ModuleState::Error;
        return false;
    }

    m_HardwareConnected = m_Bus->Probe(kI2cAddr);
    if (!m_HardwareConnected)
        DEKI_LOG_WARNING("DS3231RTC: chip did not ACK at 0x%02X on I2C port %d", kI2cAddr, m_BusPort);

    m_State = ModuleState::Initialized;
    return true;
}

void DS3231RTC::Shutdown()
{
    m_Bus = nullptr;
    m_State = ModuleState::Uninitialized;
    m_HardwareConnected = false;
}

DekiDateTime DS3231RTC::Now() const
{
    DekiDateTime dt{};
    if (!m_Bus) return dt;

    uint8_t reg[7] = {};
    if (!m_Bus->Read(kI2cAddr, 0x00, reg, 7))
        return dt;

    dt.second = BcdToBin(reg[0] & 0x7F);
    dt.minute = BcdToBin(reg[1] & 0x7F);
    dt.hour   = BcdToBin(reg[2] & 0x3F);
    dt.day    = BcdToBin(reg[4] & 0x3F);
    dt.month  = BcdToBin(reg[5] & 0x1F);
    dt.year   = (uint16_t)(2000 + BcdToBin(reg[6]));
    return dt;
}

void DS3231RTC::SetDateTime(const DekiDateTime& dt)
{
    if (!m_Bus) return;

    uint8_t reg[7] = {
        BinToBcd(dt.second),
        BinToBcd(dt.minute),
        BinToBcd(dt.hour),
        1,
        BinToBcd(dt.day),
        BinToBcd(dt.month),
        BinToBcd((uint8_t)(dt.year - 2000)),
    };
    m_Bus->Write(kI2cAddr, 0x00, reg, 7);
}
