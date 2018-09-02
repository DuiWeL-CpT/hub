#ifndef __TELNET_SERVER
#define __TELNET_SERVER

#include <string>
#include <utility>
#include "libsocket/inetserverstream.hpp"
#include "libsocket/socket.hpp"
#include "libsocket/select.hpp"
#include <thread>
#include <map>
#include <sstream>
#include <iostream>
#include "system.hpp"
#include "connection_manager.hpp"
#include "telnet_connection.hpp"

using namespace std;
using libsocket::inet_socket;
using libsocket::inet_stream;
using libsocket::inet_stream_server;
using libsocket::selectset;

namespace network
{

template <class Impl>
class TcpServer
{
private:
    string _prefix = "";
    string _host = "127.0.0.1";
    string _port = "";
    inet_stream_server * _server = nullptr;
    selectset<inet_socket> _readSet;
    bool _working = false;
    int _id = 0;
    Impl _impl;
    ConnectionManager _connectionManager;

    static int _idGenerator;
    static map<int, TcpServer *> _instances;

    void AddStream()
    {
        auto stream = this->_server->accept();
        auto descriptor = stream->getfd();
                    
        machine::System::GetLogger()->LogDebug(this->_prefix + ": new connection to sever, accepting fd: " + std::to_string(descriptor));
        this->_readSet.add_fd(*stream, LIBSOCKET_READ);

        this->_impl.AddConnection(*stream);
    }

    void RemoveStream(inet_stream * stream)
    {
        auto descriptor = stream->getfd();
        machine::System::GetLogger()->LogDebug(this->GetExtendedPrefix(descriptor) + ": client disconnected");
        this->_readSet.remove_fd(*stream);

        this->_impl.RemoveConnection(*stream);
    }

    void ListenLoop()
    {
        auto logger = machine::System::GetLogger();
        using CM = machine::ConfigurationManager;
        using CMV = CM::Variable;
        long long delay = machine::System::GetConfigurationManager()->GetResource(CMV::TcpPooling).ToInt();

        while (this->_working)
        {
            auto readyPair = this->_readSet.wait(delay);

            while(!readyPair.first.empty())
            {
                logger->LogDebug(this->_prefix + ": new trigger, proceeding");

                auto socket = readyPair.first.back();
                readyPair.first.pop_back();

                if (socket->getfd() == this->_server->getfd())
                {
                    this->AddStream();
                    continue;
                }

                this->HandleIncommingData(*socket);    
            }
        }
    }

    void HandleIncommingData(inet_socket & socket)
    {
        auto logger = machine::System::GetLogger();

        auto block = machine::System::GetMemoryManager()->GetFreeBlock();
        char * buffer = reinterpret_cast<char *>(block->GetPayload());
        auto stream = dynamic_cast<inet_stream *>(&socket);
        auto socketFd = stream->getfd();

        int bytes = 0;
        try 
        {
            bytes = stream->rcv(buffer, 128);
        }
        catch (const libsocket::socket_exception &e)
        {
            std::cout << e.mesg;
            logger->LogError(this->GetExtendedPrefix(socketFd) + ": " + e.mesg);
        }

        if (bytes > 0)
        {                    
            std::stringstream temp_stream;
            for(int i = 0; i< bytes; ++i)
                temp_stream << " " << std::hex << (int)buffer[i];
            string data = temp_stream.str();

            logger->LogDebug(this->GetExtendedPrefix(socketFd) + ": new " + std::to_string(bytes) + " bytes of data:" + data);

            string message(buffer, bytes - 1);
            logger->LogDebug(this->GetExtendedPrefix(socketFd) + ": " + message);

            block->SetPayloadLength(bytes);

            try 
            {
                auto connection = this->_connectionManager.GetConnection(socketFd);
                connection->HandleData(block);
            }
            catch (const ConnectionNotFoundException &)
            {
                logger->LogError(this->GetExtendedPrefix(socketFd) + ": connection for socket FD not found");
            }
        }
        else if (bytes == 0)
        {
            this->RemoveStream(stream);        
        }
        else
        {
            logger->LogError(this->GetExtendedPrefix(socketFd) + ": something went wrong");
        }

        machine::System::GetMemoryManager()->DeleteBlock(block->GetDescriptor());
    }

public:
    TcpServer() = delete;

    TcpServer(const string& port)
    : _port(port), _id(TcpServer::_idGenerator++), _impl(_connectionManager)
    {
        TcpServer::_instances[this->_id] = this;
        this->_prefix = "TcpServer[" + Impl::Name + "][" + std::to_string(this->_id) + "]";
    }

    ~TcpServer()
    {
        this->Stop();

        auto item = TcpServer::_instances.find(this->_id);
        TcpServer::_instances.erase(item);
    }

    inline string GetExtendedPrefix(int descriptor)
    {
        return this->_prefix + "[" + std::to_string(descriptor) + "]";
    }

    string GetPort()
    {
        return this->_port;
    }

    void SetPort(const string& port)
    {
        this->_port = port;
    }

    void SetPort(string&& port)
    {
        this->_port = port;
    }

    void Start()
    {   
        auto logger = machine::System::GetLogger();

        logger->Log(this->_prefix + ": starting on port " + this->_port);
        this->_server = new inet_stream_server(this->_host, this->_port, LIBSOCKET_IPv4);
        this->_readSet.add_fd(*(this->_server), LIBSOCKET_READ);

        this->_working = true;
        this->ListenLoop();

        logger->Logger::LogDebug(this->_prefix + ": Out of the while() loop");
    }

    void Stop()
    {
        machine::System::GetLogger()->Log(this->_prefix + ": stopped ");

        using CM = machine::ConfigurationManager;
        using CMV = CM::Variable;
        int delay = machine::System::GetConfigurationManager()->GetResource(CMV::TcpCooling).ToInt();
        
        this->_working = false;
        using DelayMS = std::chrono::duration<int, std::milli>;
        auto sleepTime = DelayMS(delay);
        this_thread::sleep_for(sleepTime);

        if (this->_server != nullptr)
        {
            this->_readSet.remove_fd(*(this->_server));
            this->_server->destroy();
            delete this->_server;
            this->_server = nullptr;
            this->_connectionManager.ClearAllConnections();
        }
    }

    static void StopAllInstances()
    {
        for (auto & pair : TcpServer<Impl>::_instances)
        {
            auto & instance = pair.second;
            instance->Stop();
        }
    }
};

template<class Impl>
map<int, TcpServer<Impl> *> TcpServer<Impl>::_instances;

template <class Impl>
int TcpServer<Impl>::_idGenerator = 0;

} // namespace network

#endif
