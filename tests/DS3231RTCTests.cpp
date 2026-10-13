// DS3231RTC against a fake I2C bus holding the chip's registers: BCD decoding
// with the control bits masked off, 12- and 24-hour modes, writing the time
// back, and what happens with no bus, no chip, or a failed read.
//
// The bus is registered through DekiI2C::RegisterBus, as I2CBusComponent does
// on a device, so the driver runs unchanged.

#include <gtest/gtest.h>

#include "chips/DS3231RTC.h"
#include "DekiI2C.h"

#include <cstring>

using namespace DekiRtc;

namespace
{

constexpr uint8_t kAddr = 0x68;

// The DS3231's register map at 0x68, and nothing anywhere else.
class FakeBus : public DekiI2c::IDekiI2C
{
public:
    uint8_t regs[19] = {};
    bool present = true;
    bool failReads = false;

    const char* GetPackageId() const override { return "i2c"; }
    const char* GetPackageName() const override { return "Fake I2C"; }
    void Configure(const Deki::PackageConfig&) override {}
    bool Initialize() override { return true; }
    void Shutdown() override {}
    void Update(float) override {}
    Deki::PackageState GetState() const override { return Deki::PackageState::Running; }
    const char* GetLastError() const override { return ""; }

    int GetPort() const override { return 0; }
    int GetSdaPin() const override { return 21; }
    int GetSclPin() const override { return 22; }
    int GetFrequencyHz() const override { return 400000; }

    bool Probe(uint8_t addr) override { return present && addr == kAddr; }
    bool Read(uint8_t addr, uint8_t reg, uint8_t* dst, size_t len) override
    {
        if (!present || failReads || addr != kAddr || reg + len > sizeof(regs))
        {
            return false;
        }
        std::memcpy(dst, regs + reg, len);
        return true;
    }
    bool Write(uint8_t addr, uint8_t reg, const uint8_t* src, size_t len) override
    {
        if (!present || addr != kAddr || reg + len > sizeof(regs))
        {
            return false;
        }
        std::memcpy(regs + reg, src, len);
        return true;
    }
};

class DS3231Test : public ::testing::Test
{
protected:
    void SetUp() override { DekiI2c::DekiI2C::RegisterBus(0, &bus); }

    void TearDown() override
    {
        rtc.Shutdown();
        DekiI2c::DekiI2C::RegisterBus(0, nullptr);
    }

    void Start(int port = 0)
    {
        Deki::PackageConfig config;
        config.settings["i2cPort"] = std::to_string(port);
        rtc.Configure(config);
        ASSERT_TRUE(rtc.Initialize());
    }

    // Seconds, minutes, hours, weekday, date, month, year, as the chip
    // stores them (BCD).
    void SetRegisters(uint8_t s, uint8_t m, uint8_t h, uint8_t wd, uint8_t d, uint8_t mo, uint8_t y)
    {
        const uint8_t r[7] = { s, m, h, wd, d, mo, y };
        std::memcpy(bus.regs, r, 7);
    }

    FakeBus bus;
    DS3231RTC rtc;
};

}  // namespace

TEST_F(DS3231Test, ReadsTheTimeFromBCD)
{
    Start();
    SetRegisters(0x59, 0x34, 0x23, 0x02, 0x13, 0x10, 0x26);
    const DekiDateTime t = rtc.Now();
    EXPECT_EQ(t.year, 2026);
    EXPECT_EQ(t.month, 10);
    EXPECT_EQ(t.day, 13);
    EXPECT_EQ(t.hour, 23);
    EXPECT_EQ(t.minute, 34);
    EXPECT_EQ(t.second, 59);
}

TEST_F(DS3231Test, ControlBitsAreNotPartOfTheValue)
{
    // Bit 7 of the month register is the century flag, and bit 7 of the
    // seconds and minutes registers is unused; none of them is a digit.
    Start();
    SetRegisters(0x80 | 0x05, 0x80 | 0x07, 0x09, 0x01, 0x01, 0x80 | 0x12, 0x99);
    const DekiDateTime t = rtc.Now();
    EXPECT_EQ(t.second, 5);
    EXPECT_EQ(t.minute, 7);
    EXPECT_EQ(t.month, 12);
    EXPECT_EQ(t.year, 2099);
}

TEST_F(DS3231Test, TwelveHourModeReadsAsTwentyFourHour)
{
    Start();
    struct Case
    {
        uint8_t reg;
        uint8_t hour;
    };
    // Bit 6: 12-hour mode. Bit 5: PM.
    const Case cases[] = {
        { 0x40 | 0x12, 0 },          // 12 AM is midnight
        { 0x40 | 0x01, 1 },          // 1 AM
        { 0x40 | 0x11, 11 },         // 11 AM
        { 0x40 | 0x20 | 0x12, 12 },  // 12 PM is noon
        { 0x40 | 0x20 | 0x01, 13 },  // 1 PM
        { 0x40 | 0x20 | 0x11, 23 },  // 11 PM
    };
    for (const Case& c : cases)
    {
        SetRegisters(0x00, 0x00, c.reg, 0x01, 0x01, 0x01, 0x26);
        EXPECT_EQ(rtc.Now().hour, c.hour) << "hours register 0x" << std::hex << int(c.reg);
    }
}

TEST_F(DS3231Test, SetDateTimeWritesBCDInTwentyFourHourMode)
{
    Start();
    DekiDateTime t;
    t.year = 2031;
    t.month = 7;
    t.day = 4;
    t.hour = 18;
    t.minute = 5;
    t.second = 42;
    rtc.SetDateTime(t);
    EXPECT_EQ(bus.regs[0], 0x42);
    EXPECT_EQ(bus.regs[1], 0x05);
    EXPECT_EQ(bus.regs[2], 0x18);  // bit 6 clear: 24-hour
    EXPECT_EQ(bus.regs[4], 0x04);
    EXPECT_EQ(bus.regs[5], 0x07);
    EXPECT_EQ(bus.regs[6], 0x31);

    const DekiDateTime back = rtc.Now();
    EXPECT_EQ(back.year, t.year);
    EXPECT_EQ(back.month, t.month);
    EXPECT_EQ(back.day, t.day);
    EXPECT_EQ(back.hour, t.hour);
    EXPECT_EQ(back.minute, t.minute);
    EXPECT_EQ(back.second, t.second);
}

TEST_F(DS3231Test, AChipThatDoesNotAnswerStillInitializes)
{
    // A missing chip is reported, not fatal: the game can still run and
    // show that the clock is not there.
    bus.present = false;
    Start();
    EXPECT_FALSE(rtc.IsHardwareConnected());
    EXPECT_EQ(rtc.Now().year, 0);
}

TEST_F(DS3231Test, AFailedReadGivesAnEmptyTime)
{
    Start();
    EXPECT_TRUE(rtc.IsHardwareConnected());
    SetRegisters(0x59, 0x34, 0x23, 0x02, 0x13, 0x10, 0x26);
    bus.failReads = true;
    const DekiDateTime t = rtc.Now();
    EXPECT_EQ(t.year, 0);
    EXPECT_EQ(t.hour, 0);
}

TEST_F(DS3231Test, NoBusOnThePortIsAnError)
{
    Deki::PackageConfig config;
    config.settings["i2cPort"] = "2";
    rtc.Configure(config);
    EXPECT_FALSE(rtc.Initialize());
    EXPECT_EQ(rtc.GetState(), Deki::PackageState::Error);
    EXPECT_STRNE(rtc.GetLastError(), "");
}

TEST_F(DS3231Test, AfterShutdownNothingIsRead)
{
    Start();
    SetRegisters(0x59, 0x34, 0x23, 0x02, 0x13, 0x10, 0x26);
    rtc.Shutdown();
    EXPECT_EQ(rtc.Now().year, 0);
    EXPECT_FALSE(rtc.IsHardwareConnected());
}
