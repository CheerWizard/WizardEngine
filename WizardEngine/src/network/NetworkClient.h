//
// Created by mecha on 31.08.2021.
//

#pragma once

#include "NetworkConfig.h"

#include "../core/String.h"

#include <asio.hpp>
#include <vector>

#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT 8080

namespace engine {

    typedef asio::ip::tcp::socket tcp_socket;
    typedef asio::ip::tcp::endpoint tcp_endpoint;
    typedef asio::io_context::work io_work;

    std::vector<char> totalBuffer(1024); // allocate 1KB per network data.

    class NetworkClient {

    public:
        NetworkClient(const std::string &host = DEFAULT_HOST,
                      const uint32_t &port = DEFAULT_PORT) :
        _host(host),
        _port(port)
        {
            create();
        }

        ~NetworkClient() {
            destroy();
        }

    private:
        void create();
        void destroy();
        void asyncRead(tcp_socket &tcpSocket);

    private:
        std::string _host;
        uint32_t _port;

    };

}