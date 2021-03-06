#ifndef __CONFIGURATION_MANAGER_HPP
#define __CONFIGURATION_MANAGER_HPP

#include <libconfig.h++>
#include <string>
#include <exception>
#include <map>

using namespace libconfig;
using namespace std;

namespace machine
{

class WrongTypeException : public exception
{

};

class IResource
{
public:
    IResource() = default;
    virtual ~IResource() {}

    virtual int ToInt() = 0;
    virtual string ToString() = 0;
    virtual bool ToBool() = 0;
};

class IntResource : public IResource
{
private:
    int _value = 0;
public:
    IntResource(int value)
    : _value(value) {}

    ~IntResource() {}

    int ToInt() override
    {
        return this->_value;
    }

    string ToString() override
    {
        throw WrongTypeException();
    }

    bool ToBool() override
    {
        throw WrongTypeException();
    }
};

class StringResource : public IResource
{
private:
    string _value = "";
public:
    StringResource(string value)
    : _value(value) {}
    ~StringResource() {}

    int ToInt() override
    {
        throw WrongTypeException();
    }

    string ToString() override
    {
        return this->_value;
    }
    
    bool ToBool() override
    {
        throw WrongTypeException();
    }
};

class BoolResource : public IResource
{
private:
    bool _value = 0;
public:
    BoolResource(int value)
    : _value(value) {}

    ~BoolResource() {}

    int ToInt() override
    {
        throw WrongTypeException();
    }

    string ToString() override
    {
        throw WrongTypeException();
    }

    bool ToBool() override
    {
        return this->_value;
    }
};

class ConfigurationManager
{
public:
    enum Variable
    {
        LogFileName,
        LogLevel,
        LogResolution,
        IsDaemon,
        PidFileName,
        TelnetEnabled,
        TelnetPort,
        TelnetSSLEnabled,
        TelnetSSLPort,
        MobileEnabled,
        MobilePort,
        SSLCRTFile,
        SSLKeyFile,
        SSLDhFile,
        SSLPassword,
        MemoryDumpName,
        MemoryBlockSize,
        MemoryPreallocatedSize,
        MemoryMaxFreePreallocatedSize,
        DatabaseFileName
    };

    bool LoadResources(string settingFile);
    bool LoadResources();
    IResource & GetResource(ConfigurationManager::Variable variableName);

    ConfigurationManager() = default;
    ~ConfigurationManager();

private:
    void ClearResources();

    Config _configFile;

    using ResourceDictionary = map<ConfigurationManager::Variable, IResource *>;
    ResourceDictionary _resources;
};

} // namespace machine

#endif
