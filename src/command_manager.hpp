#ifndef __COMMAND_MANAGER_HPP
#define __COMMAND_MANAGER_HPP

#include "icommand.hpp"
#include <map>
#include <string>

class CommandManager
{
private:
    CommandManager() = default;
    ~CommandManager();

    void ClearAllCommands();

    static CommandManager * _instance;
    std::map<std::string, ICommand *> _commands;

    ICommand * SearchCommand(const string & command);

public:
    static CommandManager * GetInstance();
    static void ClearInstance();

    void RegisterCommand(ICommand * command);
    bool ExecuteCommand(const string & command, const CommandArgument &, string & result);
    bool GetCommandHelp(const string & command, string & result);
};

#endif
