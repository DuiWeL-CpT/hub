#ifndef __UPTIME_COMMAND_TEST_HPP
#define __UPTIME_COMMAND_TEST_HPP

#include <gtest/gtest.h>
#include "uptime_command.hpp"

class UptimeCommandTest : public testing::Test
{
public:
    UptimeCommand * sut;
public:
    UptimeCommandTest()
    {
        this->sut = new UptimeCommand();
    }

    ~UptimeCommandTest()
    {
        delete this->sut;
    }
};


TEST_F(UptimeCommandTest, Register)
{
    string expect = ".uptime";
    auto result = sut->Register();
    EXPECT_STREQ(expect.c_str(), result.c_str());
}

TEST_F(UptimeCommandTest, PrintHelp)
{
    string expect = "Shows time how long service works on.\nUsage: .uptime\n";
    auto result = sut->PrintHelp();
    EXPECT_STREQ(expect.c_str(), result.c_str());
}

TEST_F(UptimeCommandTest, Execute)
{
    EXPECT_FALSE(sut->Execute());
}

TEST_F(UptimeCommandTest, GetResult)
{
    EXPECT_EQ(nullptr, sut->GetResult());
}

#endif
