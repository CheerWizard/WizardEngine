//
// Created by mecha on 31.08.2021.
//
#include "Client.h"

typedef asio::ip::tcp::socket tcp_socket;

std::vector<char> totalBuffer(1024); // allocate 1KB per network data.

void asyncRead(tcp_socket& socket) {
    socket.async_read_some(
            asio::buffer(totalBuffer.data(), totalBuffer.size()),
            [&](std::error_code errorCode, std::size_t length){
                if (!errorCode) {
                    ENGINE_INFO("Reading {0} bytes...", length);
                    for (int i = 0; i < length; ++i) {
                        ENGINE_INFO(totalBuffer[i]);
                    }

                    asyncRead(socket);
                }
            });
}

int run() {

    asio::error_code errorCode;
    asio::io_context context;

    // emulate idle , to wait for thread context will be setup.
    asio::io_context::work idleWork(context);

    // allocate network thread to run asio context.
    std::thread networkThread = std::thread([&] {
        context.run();
    });

    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127:0:0:1", errorCode), 8080);

    asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, errorCode);

    if (!errorCode) {
        ENGINE_INFO("Network client is connected!");
    } else {
        ENGINE_ERR("Failed to connect to address : {0}", errorCode.message());
    }

    if (socket.is_open()) {
        asyncRead(socket);

        std::string request = "GET /index.html HTTP/1.1\r\n"
                              "Host: example.com\r\n"
                              "Connection: close\r\n\r\n";
        socket.write_some(asio::buffer(request.data(), request.size()), errorCode);

        //delay
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2000ms);
    }

    return 0;
}