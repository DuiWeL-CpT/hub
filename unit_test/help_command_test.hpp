#ifndef __HELP_COMMAND_TEST
#define __HELP_COMMAND_TEST

#include <gtest/gtest.h>
#include "help_command.hpp"

class HelpCommandTest : public testing::Test
{
public:
    HelpCommand * sut;
public:
    HelpCommandTest()
    {
        this->sut = new HelpCommand();
    }

    ~HelpCommandTest()
    {
        delete this->sut;
    }
};

TEST_F(HelpCommandTest, Register)
{
    string expect = ".help";
    auto result = sut->Register();
    EXPECT_STREQ(expect.c_str(), result.c_str());
}

TEST_F(HelpCommandTest, PrintHelp)
{
    string expect = "Shows help info about command, i.e. .help <command>";
    auto result = sut->PrintHelp();
    EXPECT_STREQ(expect.c_str(), result.c_str());
}

TEST_F(HelpCommandTest, NoArg)
{
    CommandArgument arg;
    string expect = "Shows help info about command, i.e. .help <command>";

    EXPECT_FALSE(sut->Execute(arg));
    
    auto result = sut->GetResult();
    EXPECT_STREQ(expect.c_str(), result.c_str());
}

TEST_F(HelpCommandTest, TooManyArgs)
{
    CommandArgument arg;
    arg.Args.push_back("one");
    arg.Args.push_back("two");

    string expect = "Shows help info about command, i.e. .help <command>";

    EXPECT_FALSE(sut->Execute(arg));
    
    auto result = sut->GetResult();
    EXPECT_STREQ(expect.c_str(), result.c_str());
}

#endif