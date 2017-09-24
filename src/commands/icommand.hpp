#ifndef __I_COMMAND
#define __I_COMMAND

#include <string>
#include <vector>
#include "access_level.hpp"

using namespace std;

namespace commands
{

class CommandArgument
{
public:
    CommandArgument() = default;
    ~CommandArgument() = default;

    vector<string> Args;
    account::AccessLevel::Level RequestorAccessLevel = account::AccessLevel::Level::NotLogged;
};

class ICommand
{
public:
    virtual ~ICommand() = default;

    virtual string Register() = 0;
    virtual string PrintHelp() = 0;
    virtual bool Execute(const CommandArgument &) = 0;
    virtual string GetResult() = 0;
    virtual account::AccessLevel::Level GetAccessLevel() = 0;
};

} // namespace commands

#endif
