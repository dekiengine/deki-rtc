#include "DS3231RTC.h"
#include "DekiLogSystem.h"

#if defined(ESP32)
#include "driver/i2c.h"
#endif

namespace
{
    constexpr i2c_port_t kI2cPort =
#if defined(ESP32)
        I2C_NUM_0;
#else
        (i2c_port_t)0;
#endif

    inline uint8_t BcdToBin(uint8_t v) { return (uint8_t)((v >> 4) * 10 + (v & 0x0F)); }
    inline uint8_t BinToBcd(uint8_t v) { return (uint8_t)(((v / 10) << 4) | (v % 10)); }
}

void DS3231RTC::Configure(const ModuleConfig& config)
{
    m_PinSDA = config.GetPin("SDA", -1);
    m_PinSCL = config.GetPin("SCL", -1);
    m_I2cHz  = (uint32_t)config.GetInt("i2c_hz", 400000);
}

bool DS3231RTC::Initialize()
{
#if defined(ESP32)
    if (m_PinSDA < 0 || m_PinSCL < 0)
    {
        m_LastError = "DS3231: SDA/SCL pins not configured";
        m_State = ModuleState::Error;
        return false;
    }

    i2c_config_t conf = {};
    conf.mode             = I2C_MODE_MASTER;
    conf.sda_io_num       = (gpio_num_t)m_PinSDA;
    conf.scl_io_num       = (gpio_num_t)m_PinSCL;
    conf.sda_pullup_en    = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en    = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = m_I2cHz;

    if (i2c_param_config(kI2cPort, &conf) != ESP_OK ||
        i2c_driver_install(kI2cPort, conf.mode, 0, 0, 0) != ESP_OK)
    {
        m_LastError = "DS3231: i2c driver install failed";
        m_State = ModuleState::Error;
        return false;
    }

    m_HardwareConnected = ProbeChip();
    if (!m_HardwareConnected)
        DEKI_LOG_WARNING("DS3231RTC: chip did not ACK at 0x%02X", m_I2cAddr);

    m_State = ModuleState::Initialized;
    return true;
#else
    m_LastError = "DS3231: hardware path only built for ESP32";
    m_State = ModuleState::Error;
    return false;
#endif
}

void DS3231RTC::Shutdown()
{
#if defined(ESP32)
    i2c_driver_delete(kI2cPort);
#endif
    m_State = ModuleState::Uninitialized;
    m_HardwareConnected = false;
}

bool DS3231RTC::ProbeChip()
{
#if defined(ESP32)
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (m_I2cAddr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(kI2cPort, cmd, pdMS_TO_TICKS(50));
    i2c_cmd_link_delete(cmd);
    return ret == ESP_OK;
#else
    return false;
#endif
}

bool DS3231RTC::ReadRegisters(uint8_t startReg, uint8_t* dst, size_t len) const
{
#if defined(ESP32)
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (m_I2cAddr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, startReg, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (m_I2cAddr << 1) | I2C_MASTER_READ, true);
    if (len > 1) i2c_master_read(cmd, dst, len - 1, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, dst + len - 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(kI2cPort, cmd, pdMS_TO_TICKS(50));
    i2c_cmd_link_delete(cmd);
    return ret == ESP_OK;
#else
    (void)startReg; (void)dst; (void)len;
    return false;
#endif
}

bool DS3231RTC::WriteRegisters(uint8_t startReg, const uint8_t* src, size_t len)
{
#if defined(ESP32)
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (m_I2cAddr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, startReg, true);
    i2c_master_write(cmd, (uint8_t*)src, len, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(kI2cPort, cmd, pdMS_TO_TICKS(50));
    i2c_cmd_link_delete(cmd);
    return ret == ESP_OK;
#else
    (void)startReg; (void)src; (void)len;
    return false;
#endif
}

DekiDateTime DS3231RTC::Now() const
{
    DekiDateTime dt{};
    uint8_t reg[7] = {};
    if (!ReadRegisters(0x00, reg, 7))
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
    uint8_t reg[7] = {
        BinToBcd(dt.second),
        BinToBcd(dt.minute),
        BinToBcd(dt.hour),
        1,
        BinToBcd(dt.day),
        BinToBcd(dt.month),
        BinToBcd((uint8_t)(dt.year - 2000)),
    };
    WriteRegisters(0x00, reg, 7);
}
