#include "login_command.hpp"
#include "account_manager.hpp"
#include "system.hpp"

using namespace machine;
using namespace account;

namespace commands
{

string LoginCommand::Register()
{
    return this->_name;
}

string LoginCommand::PrintHelp()
{
    auto message = "login to system, usage: " + this->_name + " user password";
    return message;
}

bool LoginCommand::Execute(const CommandArgument & commandArgument)
{
    auto logger = System::GetLogger();

    if (commandArgument.Args.size() < 2)
    {   
        auto errorMessage = "failed, not enough arguments";
        this->_result = errorMessage;
        logger->LogError(this->_name + " " + errorMessage);
        return false;
    }
    else if (commandArgument.Args.size() > 2)
    {
        auto errorMessage = "failed, too many arguments";
        this->_result = errorMessage;
        logger->LogError(this->_name + " " + errorMessage);
        return false;
    }

    auto & name = commandArgument.Args[0];
    auto & password = commandArgument.Args[1];
    auto & user = commandArgument.Context->GetUser();

    if (!System::GetAccountManager()->ValidateUser(name, password, user))
    {
       this->_result = "access danied"; 
       return true;
    }

    user.GetAccessLevel().SetLevel(account::AccessLevel::Level::User);
    this->_result = "access granted";
    return true;
}

string LoginCommand::GetResult()
{
    return this->_result;
}

account::AccessLevel::Level LoginCommand::GetAccessLevel()
{
    return account::AccessLevel::Level::NotLogged;
}

} // namespace commands
