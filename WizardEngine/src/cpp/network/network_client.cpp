//
// Created by mecha on 25.04.2022.
//

#include <network/network_client.h>
#include <network/network_core.h>

namespace engine::network {

    using namespace core;

    namespace tcp {

        SOCKET Client::clientSocket;
        bool Client::running = false;

        thread::VoidTask<const std::string&, const s32&> Client::connectionTask = {
                "ClientConnection_Task",
                "ClientConnection_Thread",
                connectImpl
        };

        thread::VoidTask<> Client::runTask = {
                "ClientRun_Task",
                "ClientRun_Thread",
                runImpl
        };

        void Client::run() {
            runTask.run();
        }

        void Client::init() {
            clientSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (clientSocket == INVALID_SOCKET) {
                ENGINE_ERR("Client error: {0}", WSAGetLastError());
                ENGINE_THROW(tcp_client_exception("Unable to create a client socket!"));
            }
        }

        void Client::close() {
            running = false;
            closesocket(clientSocket);
        }

        void Client::connect(const std::string &ip, const s32 &port, const std::function<void()>& done) {
            connectionTask.done = done;
            connectionTask.run(ip, port);
        }

        void Client::connectImpl(const std::string &ip, const s32 &port) {
            sockaddr_in hint;
            hint.sin_family = AF_INET;
            hint.sin_port = htons(port);
            inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);
            // connect to a server
            ENGINE_INFO("Client: connecting to a server[ip:{0}, port:{1}]", ip, port);
            s32 connection = ::connect(clientSocket, (sockaddr*)&hint, sizeof(hint));
            if (connection == SOCKET_ERROR) {
                ENGINE_ERR("Client error: {0}", WSAGetLastError());
                closesocket(clientSocket);
                ENGINE_THROW(tcp_client_exception("Unable to connect to a server!"));
            }
        }

        void Client::runImpl() {
            // send and receive data
            char buffer[kb_4];
            running = true;
            while (running) {
                thread::current_sleep(1000);
                std::string data = "Hello world!";
                ENGINE_INFO("Client: Request to server {0}", data);
                // send the text
                s32 sendResult = send(clientSocket, data.c_str(), data.size() + 1, 0);
                if (sendResult != SOCKET_ERROR) {
                    // wait for response
                    ZeroMemory(buffer, kb_4);
                    s32 bytesReceived = recv(clientSocket, buffer, kb_4, 0);
                    if (bytesReceived > 0) {
                        // server response
                        ENGINE_INFO("Client: Response from server {0}", std::string(buffer, 0, bytesReceived));
                    }
                }
            }
        }

        void Client::stop() {
            running = false;
        }

        void Client::connectRun(const std::string &ip, const s32 &port) {
            connectionTask.done = runImpl;
            connectionTask.run(ip, port);
        }
    }

    namespace udp {

        SOCKET Client::clientSocket;
        sockaddr_in server;

        thread::VoidTask<const std::string&, const s32&> Client::bindTask = {
                "UDP_Client_Bind_Task",
                "UDP_Client_Bind_Thread",
                bindImpl
        };

        thread::VoidTask<const std::string&> Client::sendTask = {
                "UDP_Client_Send_Task",
                "UDP_Client_Send_Thread",
                sendImpl
        };

        void Client::init() {
            clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
            if (clientSocket == SOCKET_ERROR) {
                ENGINE_THROW(udp_client_exception("UDP_Client: Unable to create socket!"));
            }
        }

        void Client::close() {
            closesocket(clientSocket);
        }

        void Client::bind(const std::string &ip, const s32 &port) {
            bindTask.run(ip, port);
        }

        void Client::bind(const std::string &ip, const s32 &port, const std::function<void()> &done) {
            bindTask.done = done;
            bindTask.run(ip, port);
        }

        void Client::bindImpl(const std::string &ip, const s32 &port) {
            server.sin_family = AF_INET;
            server.sin_port = htons(port);
            inet_pton(AF_INET, ip.c_str(), &server.sin_addr);
        }

        void Client::send(const std::string &data) {
            sendTask.run(data);
        }

        void Client::sendImpl(const std::string &data) {
            thread::current_sleep(1000);
            s32 okStatus = sendto(
                    clientSocket, data.c_str(), data.size() + 1,
                    0, (sockaddr*) &server, sizeof(server)
            );

            if (okStatus == SOCKET_ERROR) {
                ENGINE_ERR("UDP_Client: Unable to send data, error: {0}", WSAGetLastError());
            }
        }
    }

}