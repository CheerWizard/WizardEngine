//
// Created by mecha on 24.04.2022.
//

#include <network/network_server.h>
#include <network/network_core.h>

namespace engine::network {

    using namespace core;

    namespace tcp {

        bool Server::running = false;
        ClientProfile Server::clientProfile;
        SOCKET Server::listeningSocket;

        thread::VoidTask<const s32&> Server::listenTask = {
                "ServerRun_Task",
                "ServerRun_Thread",
                listenImpl
        };

        thread::VoidTask<> Server::runTask = {
                "ServerRun_Task",
                "ServerRun_Thread",
                runImpl
        };

        void Server::init() {
            listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (listeningSocket == INVALID_SOCKET) {
                ENGINE_THROW(tcp_server_exception("Unable to create TCP listening socket!"));
            }
        }

        void Server::close() {
            running = false;
            closesocket(clientProfile.socket);
            delete clientProfile.host;
            delete clientProfile.service;
        }

        void Server::listen(const s32 &port, const std::function<void()>& done) {
            listenTask.done = done;
            listenTask.run(port);
        }

        void Server::listenRun(const s32 &port) {
            listenTask.done = runImpl;
            listenTask.run(port);
        }

        void Server::listenImpl(const s32 &port) {
            // bind IP address amd port to socket
            sockaddr_in hint;
            hint.sin_family = AF_INET;
            hint.sin_port = htons(port);
            hint.sin_addr.S_un.S_addr = INADDR_ANY;
            bind(listeningSocket, (sockaddr*)&hint, sizeof(hint));
            // set the socket for listening
            ::listen(listeningSocket, SOMAXCONN);
            // wait for connection
            sockaddr_in client;
            s32 clientSize = sizeof(client);
            SOCKET clientSocket = accept(listeningSocket, (sockaddr*)&client, &clientSize);
            if (clientSocket == INVALID_SOCKET) {
                ENGINE_THROW(tcp_server_exception("Unable to create TCP client socket!"));
            }
            // setup client profile
            char* host = new char[NI_MAXHOST];
            char* service = new char[NI_MAXSERV];

            ZeroMemory(host, NI_MAXHOST);
            ZeroMemory(service, NI_MAXSERV);
            // log client's info
            if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
                ENGINE_INFO("Client host connected on port: {0}", service);
            } else {
                inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
                ENGINE_INFO("Client host connected on port: {0}", ntohs(client.sin_port));
            }
            clientProfile = { clientSocket, host, service };
            // close listening socket
            closesocket(listeningSocket);
        }

        void Server::run() {
            runTask.run();
        }

        void Server::runImpl() {
            // receive message and send back to client
            char buffer[kb_4];
            running = true;
            while (running) {
                thread::current_sleep(1000);
                ZeroMemory(buffer, kb_4);
                // receive data from client
                s32 receivedBytes = recv(clientProfile.socket, buffer, kb_4, 0);
                ENGINE_INFO("Server: Response from client {0}", buffer);
                // check socket error
                if (receivedBytes == SOCKET_ERROR) {
                    ENGINE_ERR("Server: error during receiving data size: {0}", kb_4);
                    break;
                }
                // check client connection
                if (receivedBytes == 0) {
                    ENGINE_WARN("Client [host:{0}, service:{1}] disconnected!", clientProfile.host, clientProfile.service);
                    break;
                }
                // send data to client
                ENGINE_INFO("Server: Request to client {0}", buffer);
                send(clientProfile.socket, buffer, receivedBytes + 1, 0);
            }
        }

        void Server::stop() {
            running = false;
        }

    }

    namespace udp {

        SOCKET Server::clientSocket;
        bool Server::running = false;

        thread::VoidTask<const s32&> Server::bindTask = {
                "UDP_Server_Bind_Task",
                "UDP_Server_Bind_Thread",
                bindImpl
        };

        thread::VoidTask<> Server::runTask = {
                "UDP_Server_Run_Task",
                "UDP_Server_Run_Thread",
                runImpl
        };

        void Server::init() {
            clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
            if (clientSocket == SOCKET_ERROR) {
                ENGINE_THROW(udp_server_exception("UDP_Server: Unable to create socket!"));
            }
        }

        void Server::close() {
            running = false;
            closesocket(clientSocket);
        }

        void Server::bind(const s32 &port, const std::function<void()> &done) {
            bindTask.done = done;
            bindTask.run(port);
        }

        void Server::bindRun(const s32 &port) {
            bindTask.done = runImpl;
            bindTask.run(port);
        }

        void Server::bindImpl(const s32 &port) {
            sockaddr_in hint;
            hint.sin_family = AF_INET;
            hint.sin_port = htons(port);
            hint.sin_addr.S_un.S_addr = INADDR_ANY;
            s32 bindResult = ::bind(clientSocket, (sockaddr*)&hint, sizeof(hint));
            if (bindResult == SOCKET_ERROR) {
                ENGINE_ERR("Error: {0}", WSAGetLastError());
                ENGINE_THROW(udp_server_exception("Unable to bind to UDP socket!"));
            }
        }

        void Server::run() {
            runTask.run();
        }

        void Server::runImpl() {
            sockaddr_in client;
            s32 clientLength = sizeof(client);
            ZeroMemory(&client, clientLength);
            char buffer[kb_1];
            running = true;

            while (running) {
                thread::current_sleep(1000);
                ZeroMemory(buffer, kb_1);
                s32 bytesReceived = recvfrom(clientSocket, buffer, kb_1, 0, (sockaddr*)&client, &clientLength);
                if (bytesReceived == SOCKET_ERROR) {
                    ENGINE_ERR("UDP_Server: Error receiving data from UDP client: {0}", WSAGetLastError());
                    continue;
                }
                // get client IP
                char clientIp[256];
                ZeroMemory(clientIp, 256);
                inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);
                // display client IP and message
                ENGINE_INFO("UDP_Server: Message received from client[IP:{0}], message: {1}", clientIp, buffer);
            }
        }

        void Server::stop() {
            running = false;
        }
    }

}