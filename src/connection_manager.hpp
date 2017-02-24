#ifndef __CONNECTION_MANAGER_HPP
#define __CONNECTION_MANAGER_HPP

#include <libsocket/inetserverstream.hpp>
#include <map>
#include "iconnection.hpp"

using namespace std;
using libsocket::inet_stream;

class ConnectionManager
{
private:
    map<int, IConnection *> _connections;
public:
    ConnectionManager() = default;
    ~ConnectionManager() = default;

    template <typename CONNECTION_TYPE>
    void AddConnection(inet_stream * stream)
    {
        auto descriptor = stream->getfd();
        auto connection = new CONNECTION_TYPE(descriptor, stream, this);
        this->_connections[descriptor] = connection;
    }

    void RemoveConnection(int socketFd);
    IConnection * GetConnection(int socketFd);
    void ClearAllConnections();
};

#endif