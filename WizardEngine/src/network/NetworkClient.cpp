//
// Created by mecha on 31.08.2021.
//

#include "NetworkClient.h"

#include "../core/Logger.h"
#include "../core/Thread.h"

namespace engine {

    void NetworkClient::create() {
        asio::error_code errorCode;
        asio::io_context context;

        // emulate idle , to wait for thread context will be setup.
        io_work idleWork(context);

        // allocate network thread to run asio context.
        std::thread networkThread = std::thread([&] {
            context.run();
        });

        tcp_endpoint endpoint(asio::ip::make_address(_host, errorCode), _port);

        asio::ip::tcp::socket socket(context);

        socket.connect(endpoint, errorCode);

        if (!errorCode) {
            ENGINE_INFO("NetworkClient is connected!");
        } else {
            ENGINE_ERR("NetworkClient : Failed to connect to {0}:{1}, error : {2}",
                       _host,
                       _port,
                       errorCode.message());
        }

        if (socket.is_open()) {
            asyncRead(socket);

            std::string request = "GET /index.html HTTP/1.1\r\n"
                                  "Host: example.com\r\n"
                                  "Connection: close\r\n\r\n";
            socket.write_some(asio::buffer(request.data(), request.size()), errorCode);

            //delay
            CURRENT_THREAD_SLEEP_MILLIS(2000);
        }
    }

    void NetworkClient::destroy() {
    }

    void NetworkClient::asyncRead(tcp_socket &tcpSocket) {
        auto buffer = asio::buffer(totalBuffer.data(), totalBuffer.size());
        tcpSocket.async_read_some(buffer,[&](std::error_code errorCode, std::size_t length) {
            if (errorCode) {
                ENGINE_ERR("NetworkClient : asyncRead(), errorCode : {0}", errorCode.message());
                return;
            }

            ENGINE_INFO("NetworkClient : asyncRead(), reading {0} bytes...", length);
            for (int i = 0; i < length; ++i) {
                ENGINE_INFO(totalBuffer[i]);
            }
            asyncRead(tcpSocket);
        });
    }

}