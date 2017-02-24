#include "command_manager.hpp"
#include "logger.hpp"

CommandManager * CommandManager::_instance = nullptr;

CommandManager::~CommandManager()
{
    if (CommandManager::_instance != nullptr)
    { 
        delete CommandManager::_instance;
    }
}

CommandManager * CommandManager::GetInstance()
{
    if (CommandManager::_instance == nullptr)
    {
        CommandManager::_instance = new CommandManager();
    }

    return CommandManager::_instance;
}

void CommandManager::RegisterCommand(ICommand * command)
{
    auto key = command->Register();
    CommandManager::_commands[key] = command;

    Logger::LogDebug("CommandManager: command " + key + " registered");
}

void CommandManager::ClearAllCommands()
{
    Logger::Log("CommandManager: cleaning all commands");

    for (auto & pair : CommandManager::_commands)
    {
        delete pair.second;
    }

    CommandManager::_commands.clear();
}

bool CommandManager::ExecuteCommand(const string & command, string & result)
{
    auto item = CommandManager::_commands.find(command);
    if (item == CommandManager::_commands.end())
    {
        Logger::LogError("CommandManager: command " + command + " not found");
        return false;
    }

    if (!item->second->Execute())
    {
        Logger::LogError("CommandManager: command " + command + " executed with error");
        return false;
    }

    result = item->second->GetResult();

    return true;
}