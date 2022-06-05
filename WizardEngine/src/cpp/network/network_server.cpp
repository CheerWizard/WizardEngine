//
// Created by mecha on 24.04.2022.
//

#include <network/network_server.h>
#include <core/filesystem.h>
#include <serialization/AssetManager.h>

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
        sockaddr_in Server::client;

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
                    // unpack data and dispatch it
                    std::pair<YAML::Node, GDHeader> gdNodeHeader;
                    GDSerializer::deserialize(data, gdNodeHeader);
                    switch (gdNodeHeader.second.address) {
                        // dispatch as data for server
                        case CLIENT_TO_SERVER:
                            dispatch(gdNodeHeader.first, gdNodeHeader.second);
                            break;
                        // send received data to all clients
                        case CLIENT_TO_CLIENT:
                            send(data, receivedSize);
                            break;
                    }
                }
            }
        }

        void Server::stop() {
            listenTask.isRunning = false;
        }

        void Server::send(char *data, size_t size) {
            s32 okStatus = sendto(
                    clientSocket, data, size + 1,
                    0, (sockaddr*) &client, sizeof(client)
            );

            if (okStatus == SOCKET_ERROR) {
                u32 errorCode = socket::getLastError();
                ENGINE_ERR("UDP_Server: Sender failed. \nError: {0}", errorCode);
                listener->onUDPSenderFailed(data, size);
            }
        }

        void Server::dispatch(const YAML::Node& gdNode, const GDHeader& header) {
            switch (header.type) {
                case SERVER_SAVE_SCENE:
                    saveScene(gdNode);
                    break;
                case SERVER_LOAD_SCENE:
                    loadScene(gdNode);
                    break;
                default: break;
            }
        }

        void Server::saveScene(const YAML::Node &gdNode) {
            ENGINE_INFO("UDP_Server: saveScene");
            // deserialize into empty scene and save into asset file
            auto scene = createRef<ecs::Scene>();
            io::SceneSerializable body(scene);
            body.deserialize(gdNode);
            io::LocalAssetManager::saveScene(scene);
            // send response back to client
            GDHeader header(SERVER_TO_CLIENT, SERVER_SAVE_SCENE);
            GDResponse responseBody;
            auto response = GDSerializer::serialize(header, responseBody);
            send(response.data, response.size);
        }

        void Server::loadScene(const YAML::Node &gdNode) {
            ENGINE_INFO("UDP_Server: loadScene");
            // extract scene name
            GDString sceneName;
            sceneName.deserialize(gdNode);
            // load scene from asset manager
            auto scene = io::LocalAssetManager::loadScene(sceneName.value.c_str());
            // send scene back to client
            GDHeader header(SERVER_TO_CLIENT, SERVER_LOAD_SCENE);
            io::SceneSerializable body(scene);
            auto response = GDSerializer::serialize(header, body);
            send(response.data, response.size);
        }
    }

}