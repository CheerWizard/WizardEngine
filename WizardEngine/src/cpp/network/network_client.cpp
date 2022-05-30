//
// Created by mecha on 25.04.2022.
//

#include <network/network_client.h>

namespace engine::network {

    using namespace core;

    void RequestQueue::push(const NetworkData &networkData) {
        _queue.push(networkData);
    }

    void RequestQueue::push(GDHeader &header, GDBody &body) {
        push(GDSerializer::serialize(header, body));
    }

    void RequestQueue::pop() {
        _queue.pop();
    }

    const NetworkData& RequestQueue::front() const {
        return _queue.front();
    }

    bool RequestQueue::empty() const {
        return _queue.empty();
    }

    namespace tcp {

        void Sender::run(const SOCKET& clientSocket) {
            this->clientSocket = clientSocket;
            senderTask.name = "TCPSender_Task";
            senderTask.threadName = "TCPSender_Thread";
            senderTask.runnable = [this]() { runImpl(); };
            senderTask.run();
        }

        void Sender::stop() {
            senderTask.isRunning = false;
        }

        void Sender::runImpl() {
            while (senderTask.isRunning) {
                if (requestQueue.empty()) {
                    continue;
                }

                auto request = requestQueue.front();
                s32 okStatus = ::send(clientSocket, request.data, request.size + 1, 0);

                if (okStatus == SOCKET_ERROR) {
                    u32 errorCode = socket::getLastError();
                    ENGINE_ERR("TCP_Sender: Send failed, error: {0}", errorCode);
                    listener->onTCPSenderFailed(request.data, request.size);
                }

                listener->onTCPSenderSuccess();
                requestQueue.pop();
            }
        }

        RequestQueue& Sender::getRequestQueue() {
            return requestQueue;
        }

        void Receiver::run(const SOCKET& clientSocket) {
            this->clientSocket = clientSocket;
            receiverTask.name = "TCPReceiver_Task";
            receiverTask.threadName = "TCPReceiver_Thread";
            receiverTask.runnable = [this]() { runImpl(); };
            receiverTask.run();
        }

        void Receiver::stop() {
            receiverTask.isRunning = false;
        }

        void Receiver::runImpl() {
            char data[kb_1];

            while (receiverTask.isRunning) {
                memset(data, 0, kb_1);
                s32 receivedSize = ::recv(clientSocket, data, kb_1,0);

                if (receivedSize == SOCKET_ERROR) {
                    u32 errorCode = socket::getLastError();
                    ENGINE_ERR("TCP_Receiver: Receiver Failed. \nError = {0}", errorCode);
                    listener->onTCPReceiverFailed(data, kb_1);
                    continue;
                }

                if (receivedSize > 0) {
                    ENGINE_INFO("TCP_Receiver: Received data \nsize: {0} \ndata: {1}", receivedSize, data);
                    // unpack data
                    // todo abstract this block of code, as currently it works only for gdf. data type
                    // todo in future we should also support HTTP or other data type
                    std::pair<YAML::Node, GDHeader> gdNodeHeader;
                    GDSerializer::deserialize(data, gdNodeHeader);
                    listener->onTCPReceiverSuccess(gdNodeHeader.first, gdNodeHeader.second);
                }
            }
        }

        SOCKET Client::clientSocket;

        thread::VoidTask<const std::string&, const s32&> Client::connectionTask = {
                "TCP_ClientConnection_Task",
                "TCP_ClientConnection_Thread",
                connectImpl
        };

        ClientListener* Client::listener = nullptr;

        Ref<Sender> Client::sender;
        Ref<Receiver> Client::receiver;

        bool Client::init(
                ClientListener* clientListener,
                SenderListener* senderListener,
                ReceiverListener* receiverListener
        ) {
            listener = clientListener;
            sender = createRef<Sender>(senderListener);
            receiver = createRef<Receiver>(receiverListener);
            clientSocket = socket::open(AF_INET, SOCK_STREAM, 0);

            if (clientSocket == INVALID_SOCKET) {
                u32 errorCode = socket::getLastError();
                ENGINE_ERR("TCP_Client: Unable to create a client socket! Unknown error={0}", errorCode);
                listener->onTCPSocketClosed();
                return false;
            }

            return true;
        }

        void Client::close() {
            connectionTask.isRunning = false;
            socket::close_socket(clientSocket);
            listener->onTCPSocketClosed();
        }

        void Client::connect(const std::string &ip, const s32 &port) {
            connectionTask.run(ip, port);
        }

        void Client::connectImpl(const std::string &ip, const s32 &port) {
            ENGINE_INFO("TCP_Client: connecting to a server[ip:{0}, port:{1}]", ip, port);
            s32 connection = socket::connect(clientSocket, AF_INET, ip.c_str(), port);
            // handle connection error and retry to connect!
            if (connection == SOCKET_ERROR) {
                u32 errorCode = socket::getLastError();
                switch (errorCode) {
                    case 10038 :
                        ENGINE_ERR("TCP_Client: Unable to connect to server");
                        listener->onTCPConnectionFailed();
                        break;
                    default :
                        ENGINE_ERR("TCP_Client: Unable to create a connection socket! Unknown error={0}", errorCode);
                        listener->onTCPConnectionFailed();
                        break;
                }

                u32 retrySleepMs = 2000;
                ENGINE_WARN("TCP_Client: Retry to connect to a server after {0} ms!", retrySleepMs);
                thread::current_sleep(retrySleepMs);
                connectImpl(ip, port);
            }

            sender->run(clientSocket);
            receiver->run(clientSocket);
        }

        RequestQueue& Client::getRequestQueue() {
            return sender->getRequestQueue();
        }
    }

    namespace udp {

        void Sender::run(const SOCKET& clientSocket, const sockaddr_in& server) {
            this->clientSocket = clientSocket;
            this->server = server;
            senderTask.name = "UDPSender_Task";
            senderTask.threadName = "UDPSender_Thread";
            senderTask.runnable = [this]() { runImpl(); };
            senderTask.run();
        }

        void Sender::stop() {
            senderTask.isRunning = false;
        }

        void Sender::runImpl() {
            while (senderTask.isRunning) {
                if (requestQueue.empty()) {
                    continue;
                }

                auto request = requestQueue.front();
                s32 okStatus = sendto(
                        clientSocket, request.data, request.size + 1,
                        0, (sockaddr*) &server, sizeof(server)
                );

                if (okStatus == SOCKET_ERROR) {
                    u32 errorCode = socket::getLastError();
                    ENGINE_ERR("UDP_Sender: Unable to send data, error: {0}", errorCode);
                    listener->onUDPSenderFailed(request.data, request.size);
                }

                listener->onUDPSenderSuccess();
                requestQueue.pop();
            }
        }

        RequestQueue& Sender::getRequestQueue() {
            return requestQueue;
        }

        void Receiver::run(const SOCKET& clientSocket, const sockaddr_in& server) {
            this->clientSocket = clientSocket;
            this->server = server;
            receiverTask.name = "UDPReceiver_Task";
            receiverTask.threadName = "UDPReceiver_Thread";
            receiverTask.runnable = [this]() { runImpl(); };
            receiverTask.run();
        }

        void Receiver::stop() {
            receiverTask.isRunning = false;
        }

        void Receiver::runImpl() {
            s32 serverLength = sizeof(server);
            memset(&server, 0, serverLength);
            char data[kb_1];

            while (receiverTask.isRunning) {
                memset(data, 0, kb_1);
                s32 receivedSize = socket::receiveFrom(clientSocket, data, kb_1,0, server);

                if (receivedSize == SOCKET_ERROR) {
                    u32 errorCode = socket::getLastError();
                    ENGINE_ERR("UDP_Receiver: Receiver Failed. \nError = {0}", errorCode);
                    listener->onUDPReceiverFailed(data, kb_1);
                    continue;
                }

                if (receivedSize > 0) {
                    ENGINE_INFO("UDP_Receiver: Received data \nsize: {0} \ndata: {1}", receivedSize, data);
                    // unpack data
                    // todo abstract this block of code, as currently it works only for gdf. data type
                    // todo in future we should also support HTTP or other data type
                    std::pair<YAML::Node, GDHeader> gdNodeHeader;
                    GDSerializer::deserialize(data, gdNodeHeader);
                    listener->onUDPReceiverSuccess(gdNodeHeader.first, gdNodeHeader.second);
                }
            }
        }

        SOCKET Client::clientSocket;
        sockaddr_in Client::server;

        thread::VoidTask<const std::string&, const s32&> Client::connectionTask = {
                "UDPClientConnection_Task",
                "UDPClientConnection_Thread",
                connectImpl
        };

        ClientListener* Client::listener = nullptr;
        Ref<Sender> Client::sender;
        Ref<Receiver> Client::receiver;

        bool Client::init(
                ClientListener* clientListener,
                SenderListener* senderListener,
                ReceiverListener* receiverListener
        ) {
            listener = clientListener;
            sender = createRef<Sender>(senderListener);
            receiver = createRef<Receiver>(receiverListener);
            clientSocket = socket::open(AF_INET, SOCK_DGRAM, 0);

            if (clientSocket == SOCKET_ERROR) {
                u32 errorCode = socket::getLastError();
                ENGINE_ERR("UDP_Client: Unable to create a client socket! Unknown error={0}", errorCode);
                listener->onUDPSocketClosed();
                return false;
            }

            return true;
        }

        void Client::close() {
            sender->stop();
            receiver->stop();
            socket::close_socket(clientSocket);
            listener->onUDPSocketClosed();
        }

        void Client::connect(const std::string &ip, const s32 &port) {
            connectionTask.run(ip, port);
        }

        void Client::connectImpl(const std::string &ip, const s32 &port) {
            server.sin_family = AF_INET;
            server.sin_port = htons(port);
            inet_pton(AF_INET, ip.c_str(), &server.sin_addr);

            sender->run(clientSocket, server);
            receiver->run(clientSocket, server);
        }

        RequestQueue& Client::getRequestQueue() {
            return sender->getRequestQueue();
        }
    }

}