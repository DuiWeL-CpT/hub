#ifndef __LOGIN_COMMAND_TEST_HPP
#define __UPTIME_COMMAND_TEST_HPP

#include <gtest/gtest.h>
#include "login_command.hpp"
#include <iostream>
#include <thread>

using commands::LoginCommand;
using commands::CommandArgument;

class LoginCommandTest : public testing::Test
{
public:
    LoginCommand * sut;
public:
    LoginCommandTest()
    {
        this->sut = new LoginCommand();
    }

    ~LoginCommandTest()
    {
        delete this->sut;
    }
};


TEST_F(LoginCommandTest, Register)
{
    string expect = ".login";
    auto result = sut->Register();
    EXPECT_STREQ(expect.c_str(), result.c_str());
}

TEST_F(LoginCommandTest, PrintHelp)
{
    string expect = "login to system, usage: .login user password";
    auto result = sut->PrintHelp();
    EXPECT_STREQ(expect.c_str(), result.c_str());
}

TEST_F(LoginCommandTest, ExecuteTooLessParameters)
{
    string expect = "failed, not enough arguments";
    CommandArgument arg; // arg.Args.size < 2

    EXPECT_FALSE(sut->Execute(arg));
    auto result = sut->GetResult();
    
    EXPECT_STREQ(expect.c_str(), result.c_str());
}

TEST_F(LoginCommandTest, ExecuteTooManyParameters)
{
    string expect = "failed, too many arguments";
    CommandArgument arg;
    arg.Args.push_back("arg1");
    arg.Args.push_back("arg2");
    arg.Args.push_back("arg3");

    EXPECT_FALSE(sut->Execute(arg));
    auto result = sut->GetResult();
    
    EXPECT_STREQ(expect.c_str(), result.c_str());
}

TEST_F(LoginCommandTest, AccessLevel)
{
    EXPECT_EQ(account::AccessLevel::Level::NotLogged, sut->GetAccessLevel());
}

#endif