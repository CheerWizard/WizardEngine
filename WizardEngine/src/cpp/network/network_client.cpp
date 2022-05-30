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
        vector<NetworkData> Client::requests;

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
            // handle connection error and retry to connect!
            if (connection == SOCKET_ERROR) {
                ENGINE_WARN("TCP_Client: Connection error. Closing client socket!");
                socket::close_socket(clientSocket);
                listener->tcp_socketClosed();

                u32 errorCode = socket::getLastError();
                switch (errorCode) {
                    case 10038 :
                        ENGINE_ERR("TCP_Client: Unable to connect to server");
                        listener->tcp_connectionFailed();
                        break;
                    default :
                        ENGINE_ERR("Unable to create a connection socket! Unknown error={0}", errorCode);
                        listener->tcp_connectionFailed();
                        break;
                }

                u32 retrySleepMs = 2000;
                ENGINE_WARN("TCP_Client: Retry to connect to a server after {0} ms!", retrySleepMs);
                thread::current_sleep(retrySleepMs);
                connectImpl(ip, port);
            }

            runImpl();
        }

        void Client::send(const NetworkData& networkData, const std::function<void(char*)>& receiver) {
            // send and receive data
            char* data = strdup(networkData.data);
            size_t size = networkData.size;
            ENGINE_INFO("TCP_Client: Send data to a server \n{0}", data);
            s32 sendResult = ::send(clientSocket, data, size + 1, 0);
            if (sendResult != SOCKET_ERROR) {
                // wait for response
                memset(data, 0, size);
                // todo recv() returns correct size but incorrect data buffer. ex: data=".5" size=47
                s32 sizeReceived = recv(clientSocket, data, size, 0);
                if (sizeReceived > 0) {
                    // server response
                    ENGINE_INFO("TCP_Client: Received data from a server \nsize:{0}\n{1}", sizeReceived, data);
                    receiver(data);
                }
            }
        }

        void Client::runImpl() {
            isRunning = true;
            while (isRunning) {
                if (requests.empty()) {
                    u32 sleepMs = 1000;
                    ENGINE_WARN("TCP_Client: No requests in queue. Sleeping thread on {0} ms!", sleepMs);
                    thread::current_sleep(sleepMs);
                    continue;
                }

                send(requests.at(0), [](char* data) {
                    // todo make client abstract, as this implementation is specific for .gdf data
                    // todo it will not suit for HTTP or any other client
                    requests.erase(requests.begin());
                    std::pair<YAML::Node, GDHeader> gdNodeHeader;
                    bool success = GDSerializer::deserialize(data, gdNodeHeader);
                    if (success) {
                        listener->onGameDataReceived(gdNodeHeader);
                    }
                });
            }
        }

        void Client::pushRequest(const NetworkData& networkData) {
            ENGINE_INFO("TCP_Client: Pushing network data \nsize: {0}, \ndata: \n{1}", networkData.size, networkData.data);
            requests.emplace_back(networkData);
        }

        void Client::pushRequest(GDHeader &header, GDBody &body) {
            pushRequest(GDSerializer::serialize(header, body));
        }

        void Client::pushRequestTask(GDHeader& header, GDBody& body) {
            thread::VoidTask<> pushRequestTask = {
                    "TCPPushRequest_Task",
                    "TCPPushRequest_Thread",
                    [&header, &body]() { pushRequest(header, body); }
            };
            pushRequestTask.run();
        }

        void Client::clearRequests() {
            requests.clear();
        }
    }

    namespace udp {

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
                    listener->onSenderFailed(request.data, request.size);
                }

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
                    listener->onReceiverFailed(data, kb_1);
                    continue;
                }

                if (receivedSize > 0) {
                    ENGINE_INFO("UDP_Receiver: Received data \nsize:{0}\n{1}", receivedSize, data);
                    // unpack data
                    // todo abstract this block of code, as currently it works only for gdf. data type
                    // todo in future we should also support HTTP or other data type
                    std::pair<YAML::Node, GDHeader> gdNodeHeader;
                    GDSerializer::deserialize(data, gdNodeHeader);
                    listener->onReceiverSuccess(gdNodeHeader.first, gdNodeHeader.second);
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