//
// Created by mecha on 24.04.2022.
//

#include <network/network_server.h>

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

        ServerListener* Server::listener = nullptr;

        void Server::init(ServerListener* serverListener) {
            listener = serverListener;
            listeningSocket = socket::open(AF_INET, SOCK_STREAM, 0);
            if (listeningSocket == INVALID_SOCKET) {
                u32 errorCode = socket::getLastError();
                ENGINE_ERR("TCP_Server: Unable to create listening socket. Unknown error = {0}", errorCode);
                listener->tcp_socketNotCreated();
            }
        }

        void Server::close() {
            running = false;
            socket::close_socket(clientProfile.socket);
            listener->tcp_socketClosed();
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
            socket::listen(listeningSocket, AF_INET, port, INADDR_ANY, SOMAXCONN);
            // wait for connection
            sockaddr_in client;
            SOCKET clientSocket = socket::accept(listeningSocket, client);
            if (clientSocket == INVALID_SOCKET) {
                u32 errorCode = socket::getLastError();
                ENGINE_ERR("TCP_Server: Unable to create a client socket. Unknown error = {0}", errorCode);
                listener->tcp_clientSocketNotAccepted();
            }
            socket::SocketProfile socketProfile = socket::getSocketProfile(clientSocket, client);
            clientProfile = { clientSocket, socketProfile.host, socketProfile.service };
            // close listening socket
            socket::close_socket(listeningSocket);
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
                memset(buffer, 0, kb_4);
                // receive data from client
                s32 receivedBytes = recv(clientProfile.socket, buffer, kb_4, 0);
                ENGINE_INFO("TCP_Server: Response from client \n{0}", buffer);
                // check socket error
                if (receivedBytes == SOCKET_ERROR) {
                    ENGINE_ERR("TCP_Server: error during receiving data size: {0}", kb_4);
                    listener->tcp_receiveDataFailed(buffer, kb_4);
                    continue;
                }
                // check client connection
                if (receivedBytes == 0) {
                    ENGINE_WARN("TCP_Server: Client [host:{0}, service:{1}] disconnected!", clientProfile.host, clientProfile.service);
                    listener->tcp_clientDisconnected();
                    continue;
                }
                // unpack received data
                // todo consider to abstract unpacking as this works specifically for .gdf data
                // todo for HTTP or other type of data it won't work
                auto gdNodeHeader = GDSerializer::deserialize(buffer);
                YAML::Node gdNode = gdNodeHeader.first;
                GDHeader header = gdNodeHeader.second;
                // dispatch received data, using header address
                switch (header.address) {
                    case CLIENT_TO_SERVER:
                        // save into db, load world, sync players, etc.
                        break;
                    default: break;
                }
                // send data to client
                ENGINE_INFO("TCP_Server: Request to client \n{0}", buffer);
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

        ServerListener* Server::listener = nullptr;

        void Server::init(ServerListener* serverListener) {
            listener = serverListener;
            clientSocket = socket::open(AF_INET, SOCK_DGRAM, 0);
            if (clientSocket == SOCKET_ERROR) {
                u32 errorCode = socket::getLastError();
                ENGINE_ERR("UDP_Server: Unable to create client socket. Unknown error = {0}", errorCode);
                listener->udp_socketNotCreated();
            }
        }

        void Server::close() {
            running = false;
            socket::close_socket(clientSocket);
            listener->udp_socketClosed();
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
            s32 bindResult = socket::bind(clientSocket, AF_INET, port, INADDR_ANY);
            if (bindResult == SOCKET_ERROR) {
                u32 errorCode = socket::getLastError();
                ENGINE_ERR("UDP_Server: Unable to bind to a client socket. Unknown error = {0}", errorCode);
                listener->udp_socketBindFailed();
            }
        }

        void Server::run() {
            runTask.run();
        }

        void Server::runImpl() {
            sockaddr_in client;
            s32 clientLength = sizeof(client);
            memset(&client, 0, clientLength);
            char buffer[kb_1];
            running = true;

            while (running) {
                thread::current_sleep(1000);
                memset(buffer, 0, kb_1);
                s32 bytesReceived = socket::receiveFrom(clientSocket, buffer, kb_1,0, client);
                if (bytesReceived == SOCKET_ERROR) {
                    u32 errorCode = socket::getLastError();
                    ENGINE_ERR("UDP_Server: Error receiving data from a client. Error = ", errorCode);
                    listener->udp_receiveDataFailed(buffer, kb_1);
                    continue;
                }
                // get client IP
                char clientIp[256];
                memset(clientIp, 0, 256);
                inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);
                // display client IP and message
                ENGINE_INFO("UDP_Server: Message received from a client[IP:{0}], message: {1}", clientIp, buffer);
            }
        }

        void Server::stop() {
            running = false;
        }
    }

}