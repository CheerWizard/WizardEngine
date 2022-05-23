//
// Created by mecha on 25.04.2022.
//

#include <network/network_client.h>

namespace engine::network {

    using namespace core;

    namespace tcp {

        SOCKET Client::clientSocket;

        thread::VoidTask<const std::string&, const s32&> Client::connectionTask = {
                "ClientConnection_Task",
                "ClientConnection_Thread",
                connectImpl
        };

        thread::VoidTask<> Client::runTask = {
                "TCPClientRun_Task",
                "TCPClientRun_Thread",
                runImpl
        };

        ClientListener* Client::listener = nullptr;
        bool Client::isRunning = false;
        queue<NetworkData> Client::requests;

        void Client::init(ClientListener* clientListener) {
            listener = clientListener;
            clientSocket = socket::open(AF_INET, SOCK_STREAM, 0);
            if (clientSocket == INVALID_SOCKET) {
                u32 errorCode = socket::getLastError();
                ENGINE_ERR("TCP_Client: Unable to create a client socket! Unknown error={0}", errorCode);
                listener->tcp_socketNotCreated();
            }
        }

        void Client::close() {
            isRunning = false;
            socket::close_socket(clientSocket);
            listener->tcp_socketClosed();
        }

        void Client::connect(const std::string &ip, const s32 &port) {
            connectionTask.done = []() { listener->tcp_connectionSucceeded(); };
            connectionTask.run(ip, port);
        }

        void Client::connectImpl(const std::string &ip, const s32 &port) {
            ENGINE_INFO("TCP_Client: connecting to a server[ip:{0}, port:{1}]", ip, port);
            s32 connection = socket::connect(clientSocket, AF_INET, ip.c_str(), port);
            if (connection == SOCKET_ERROR) {
                ENGINE_WARN("TCP_Client: Connection error. Closing client socket!");
                socket::close_socket(clientSocket);
                listener->tcp_socketClosed();

                u32 errorCode = socket::getLastError();
                switch (errorCode) {
                    case 10031 :
                        ENGINE_ERR("TCP_Client: Unable to connect to server");
                        listener->tcp_connectionFailed();
                        break;
                    default :
                        ENGINE_ERR("Unable to create a connection socket! Unknown error={0}", errorCode);
                        listener->tcp_connectionFailed();
                        break;
                }
            }
        }

        void Client::send(char *data, size_t size) {
            send(data, size, [](char* data, size_t size) { listener->tcp_dataReceived(data, size); });
        }

        void Client::send(char *data, size_t size, const std::function<void(char*, size_t)>& receiver) {
            // send and receive data
            ENGINE_INFO("TCP_Client: Send data to a server {0}", data);
            s32 sendResult = ::send(clientSocket, data, size + 1, 0);
            if (sendResult != SOCKET_ERROR) {
                // wait for response
                memset(data, 0, size);
                s32 sizeReceived = recv(clientSocket, data, size, 0);
                if (sizeReceived > 0) {
                    // server response
                    ENGINE_INFO("TCP_Client: Received data from a server {0}", std::string(data, 0, sizeReceived));
                    listener->tcp_dataReceived(data, sizeReceived);
                    receiver(data, sizeReceived);
                }
            }
        }

        void Client::run() {
            isRunning = true;
            runTask.run();
        }

        void Client::runImpl() {
            while (isRunning) {
                if (requests.empty()) continue;

                auto request = requests.front();
                send(request.data, request.size);
                requests.pop();
            }
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

        ClientListener* Client::listener = nullptr;

        void Client::init(ClientListener* clientListener) {
            listener = clientListener;
            clientSocket = socket::open(AF_INET, SOCK_DGRAM, 0);
            if (clientSocket == SOCKET_ERROR) {
                u32 errorCode = socket::getLastError();
                ENGINE_ERR("UDP_Client: Unable to create a client socket! Unknown error={0}", errorCode);
                listener->udp_socketNotCreated();
            }
        }

        void Client::close() {
            socket::close_socket(clientSocket);
            listener->udp_socketClosed();
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
                u32 errorCode = socket::getLastError();
                ENGINE_ERR("UDP_Client: Unable to send data, error: {0}", errorCode);
                listener->udp_sendDataFailed(data);
            }
        }
    }

}