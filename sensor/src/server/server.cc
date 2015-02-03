#include "server.h"
#include "classes/logger.h"

Server::Server(asio::io_service& io_service, const tcp::endpoint& endpoint) : acceptor(io_service, endpoint),
socket(io_service)
{
    Accept();
}

ConnectionManager & Server::GetConnectionManager()
{
    return connectionManager;
}

void Server::Accept()
{
    acceptor.async_accept(socket,
        [this](std::error_code ec)
    {
        if (!ec)
        {
            std::make_shared<ClientSession>(std::move(socket), connectionManager)->Start();
            Logger::Log(LogLevel::Info) << "Server::Accept()";
        }

        Accept();
    });
}


