//
// Created by mecha on 25.04.2022.
//

#pragma once

#include <core/queue.h>
#include <thread/Task.h>
#include <network/socket.h>
#include <network/gdp.h>

#define TCP_CLIENT_INIT(listener) engine::network::tcp::Client::init(listener)
#define TCP_CLIENT_CONNECT(ip, port) engine::network::tcp::Client::connect(ip, port)
#define TCP_GDP_REQUEST(header, body) engine::network::tcp::Client::pushRequest(header, body)
#define TCP_NETWORK_REQUEST(data) engine::network::tcp::Client::pushRequest(data)
#define TCP_CLIENT_CLOSE() engine::network::tcp::Client::close()

namespace engine::network {

    using namespace engine::core;

    namespace tcp {

        decl_exception(tcp_client_exception)

        class ClientListener {
        public:
            virtual void tcp_socketNotCreated() = 0;
            virtual void tcp_connectionFailed() = 0;
            virtual void tcp_connectionSucceeded() = 0;
            virtual void tcp_socketClosed() = 0;
            virtual void onGameDataReceived(const std::pair<YAML::Node, GDHeader>& gdNodeHeader) = 0;
        };

        class Client final {

        private:
            Client() = default;
            ~Client() = default;

        public:
            static void init(ClientListener* clientListener);
            static void close();

            static void connect(const std::string& ip, const s32& port);

            static void send(const NetworkData& networkData, const std::function<void(char*)>& receiver);

            static void pushRequest(const NetworkData& networkData);
            static void pushRequestTask(GDHeader& header, GDBody& body);
            static void pushRequest(GDHeader& header, GDBody& body);
            static void clearRequests();

        private:
            static void connectImpl(const std::string& ip, const s32& port);
            static void runImpl();

        private:
            static SOCKET clientSocket;
            static thread::VoidTask<const std::string&, const s32&> connectionTask;
            static thread::VoidTask<> runTask;
            static ClientListener* listener;
            static bool isRunning;
            static vector<NetworkData> requests;
        };
    }

    namespace udp {

        class SenderListener {
        public:
            virtual void onSenderFailed(char* data, size_t size) = 0;
            virtual void onSenderSuccess() = 0;
        };

        class RequestQueue {

        public:
            void push(const NetworkData& networkData);
            void push(GDHeader& header, GDBody& body);
            void pop();
            [[nodiscard]] const NetworkData& front() const;
            [[nodiscard]] bool empty() const;

        private:
            queue<NetworkData> _queue;
        };

        class Sender {

        public:
            Sender(SenderListener* listener) : listener(listener) {}
            ~Sender() = default;

        public:
            void run(const SOCKET& clientSocket, const sockaddr_in& server);
            void stop();
            RequestQueue& getRequestQueue();

        private:
            void runImpl();

        private:
            SOCKET clientSocket;
            sockaddr_in server;
            SenderListener* listener;
            thread::VoidTask<> senderTask;
            RequestQueue requestQueue;
        };

        class ReceiverListener {
        public:
            virtual void onReceiverFailed(char* data, size_t size) = 0;
            virtual void onReceiverSuccess(const YAML::Node& gdNode, const GDHeader& header) = 0;
        };

        class Receiver {

        public:
            Receiver(ReceiverListener* listener) : listener(listener) {}

        public:
            void run(const SOCKET& clientSocket, const sockaddr_in& server);
            void stop();

        private:
            void runImpl();

        private:
            SOCKET clientSocket;
            sockaddr_in server;
            ReceiverListener* listener;
            thread::VoidTask<> receiverTask;
        };

        class ClientListener {
        public:
            virtual void onUDPSocketCreated() = 0;
            virtual void onUDPSocketClosed() = 0;
        };

        class Client final {

        public:
            static bool init(
                    ClientListener* clientListener,
                    SenderListener* senderListener,
                    ReceiverListener* receiverListener
            );
            static void close();
            static void connect(const std::string& ip, const s32& port);
            static RequestQueue& getRequestQueue();

        private:
            static void connectImpl(const std::string& ip, const s32& port);

        private:
            static SOCKET clientSocket;
            static sockaddr_in server;
            static thread::VoidTask<const std::string&, const s32&> connectionTask;
            static ClientListener* listener;
            static Ref<Sender> sender;
            static Ref<Receiver> receiver;
        };

    }

}
