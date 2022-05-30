//
// Created by mecha on 24.04.2022.
//

#include <network/network_server.h>

namespace engine::network {

    using namespace core;

    namespace tcp {

        ClientProfile Server::clientProfile;
        SOCKET Server::listeningSocket;

        thread::VoidTask<const s32&> Server::listenTask = {
                "TCPServerListen_Task",
                "TCPServerListen_Thread",
                listenImpl
        };

        ServerListener* Server::listener = nullptr;

        bool Server::init(ServerListener* serverListener) {
            listener = serverListener;
            listeningSocket = socket::open(AF_INET, SOCK_STREAM, 0);

            if (listeningSocket == INVALID_SOCKET) {
                u32 errorCode = socket::getLastError();
                ENGINE_ERR("TCP_Server: Unable to create listening socket. Unknown error = {0}", errorCode);
                listener->onTCPSocketClosed();
                return false;
            }

            return true;
        }

        void Server::close() {
            listenTask.isRunning = false;
            socket::close_socket(clientProfile.socket);
            listener->onTCPSocketClosed();
            delete clientProfile.host;
            delete clientProfile.service;
        }

        void Server::listen(const s32 &port) {
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
                listener->onTCPSocketUnaccepted();
            }
            socket::SocketProfile socketProfile = socket::getSocketProfile(clientSocket, client);
            clientProfile = { clientSocket, socketProfile.host, socketProfile.service };
            // close listening socket
            socket::close_socket(listeningSocket);
            runImpl();
        }

        void Server::runImpl() {
            char data[kb_4];

            while (listenTask.isRunning) {
                memset(data, 0, kb_4);
                // receive data from client
                s32 receivedSize = recv(clientProfile.socket, data, kb_4, 0);
                ENGINE_INFO("TCP_Server: Received data \nsize: {0} \ndata: {1}", receivedSize, data);
                // check socket error
                if (receivedSize == SOCKET_ERROR) {
                    ENGINE_ERR("TCP_Server: Receiver failed. \nsize: {0} \ndata: {1}", receivedSize, data);
                    listener->onTCPReceiverFailed(data, receivedSize);
                    continue;
                }
                // check client connection
                if (receivedSize == 0) {
                    ENGINE_WARN("TCP_Server: Client [host:{0}, service:{1}] disconnected!",
                                clientProfile.host, clientProfile.service);
                    listener->onTCPDisconnected();
                    continue;
                }
                // unpack received data
                // todo consider to abstract unpacking as this works specifically for .gdf data
                // todo for HTTP or other type of data it won't work
                std::pair<YAML::Node, GDHeader> gdNodeHeader;
                bool success = GDSerializer::deserialize(data, gdNodeHeader);
                if (success) {
                    YAML::Node gdNode = gdNodeHeader.first;
                    GDHeader header = gdNodeHeader.second;
                    // dispatch received data, using header address
                    switch (header.address) {
                        case CLIENT_TO_SERVER:
                            // save into db, load world, sync players, etc.
                            break;
                        default: break;
                    }
                }
                // send data to a client
                ENGINE_INFO("TCP_Server: Send data to a client \nsize: {0} \ndata:{1}", receivedSize, data);
                send(clientProfile.socket, data, receivedSize + 1, 0);
            }
        }

        void Server::stop() {
            listenTask.isRunning = false;
        }
    }

    namespace udp {

        SOCKET Server::clientSocket;

        thread::VoidTask<const s32&> Server::listenTask = {
                "UDPServerConnection_Task",
                "UDPServerConnection_Thread",
                listenImpl
        };

        ServerListener* Server::listener = nullptr;

        bool Server::init(ServerListener* serverListener) {
            listener = serverListener;
            clientSocket = socket::open(AF_INET, SOCK_DGRAM, 0);

            if (clientSocket == SOCKET_ERROR) {
                u32 errorCode = socket::getLastError();
                ENGINE_ERR("UDP_Server: Unable to create client socket. Unknown error = {0}", errorCode);
                listener->onUDPSocketClosed();
                return false;
            }

            return true;
        }

        void Server::close() {
            listenTask.isRunning = false;
            socket::close_socket(clientSocket);
            listener->onUDPSocketClosed();
        }

        void Server::listen(const s32 &port) {
            listenTask.run(port);
        }

        void Server::listenImpl(const s32 &port) {
            s32 bindResult = socket::bind(clientSocket, AF_INET, port, INADDR_ANY);

            if (bindResult == SOCKET_ERROR) {
                u32 errorCode = socket::getLastError();
                ENGINE_ERR("UDP_Server: Unable to bind to a client socket. Unknown error = {0}", errorCode);
                listener->onUDPConnectionFailed();
                u32 retryMs = 2000;
                ENGINE_WARN("UDP_Server: Retry to bind to a client socket after {0} ms!", retryMs);
                thread::current_sleep(retryMs);
                listenImpl(port);
            }

            runImpl();
        }

        void Server::runImpl() {
            sockaddr_in client;
            s32 clientLength = sizeof(client);
            memset(&client, 0, clientLength);
            char data[kb_1];

            while (listenTask.isRunning) {
                memset(data, 0, kb_1);
                s32 receivedSize = socket::receiveFrom(clientSocket, data, kb_1,0, client);
                if (receivedSize == SOCKET_ERROR) {
                    u32 errorCode = socket::getLastError();
                    ENGINE_ERR("UDP_Server: Receiver failed. \nError: {0}", errorCode);
                    listener->onUDPReceiverFailed(data, kb_1);
                    continue;
                }
                // get client IP
                char clientIp[256];
                memset(clientIp, 0, 256);
                inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);
                // display client IP and received data
                ENGINE_INFO("UDP_Server: Received data \nIP:{0} \nsize: {1} \ndata: {2}",
                            clientIp, receivedSize, data);

                if (receivedSize > 0) {
                    // send received data to all clients
                    s32 okStatus = sendto(
                            clientSocket, data, receivedSize + 1,
                            0, (sockaddr*) &client, clientLength
                    );

                    if (okStatus == SOCKET_ERROR) {
                        u32 errorCode = socket::getLastError();
                        ENGINE_ERR("UDP_Server: Sender failed. \nError: {0}", errorCode);
                        listener->onUDPSenderFailed(data, receivedSize);
                    }
                }
            }
        }

        void Server::stop() {
            listenTask.isRunning = false;
        }
    }

}