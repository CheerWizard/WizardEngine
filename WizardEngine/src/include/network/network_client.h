//
// Created by mecha on 25.04.2022.
//

#pragma once

#include <core/queue.h>
#include <thread/Task.h>
#include <network/socket.h>
#include <network/gdp.h>

namespace engine::network {

    using namespace engine::core;

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

    namespace tcp {

        class SenderListener {
        public:
            virtual void onTCPSenderFailed(char* data, size_t size) = 0;
            virtual void onTCPSenderSuccess() = 0;
        };

        class Sender {

        public:
            Sender(SenderListener* listener) : listener(listener) {}
            ~Sender() = default;

        public:
            void run(const SOCKET& clientSocket);
            void stop();
            RequestQueue& getRequestQueue();

        private:
            void runImpl();

        private:
            SOCKET clientSocket;
            SenderListener* listener;
            thread::VoidTask<> senderTask;
            RequestQueue requestQueue;
        };

        class ReceiverListener {
        public:
            virtual void onTCPReceiverFailed(char* data, size_t size) = 0;
            virtual void onTCPReceiverSuccess(const YAML::Node& gdNode, const GDHeader& header) = 0;
        };

        class Receiver {

        public:
            Receiver(ReceiverListener* listener) : listener(listener) {}

        public:
            void run(const SOCKET& clientSocket);
            void stop();

        private:
            void runImpl();

        private:
            SOCKET clientSocket;
            ReceiverListener* listener;
            thread::VoidTask<> receiverTask;
        };


        class ClientListener {
        public:
            virtual void onTCPSocketCreated() = 0;
            virtual void onTCPConnectionFailed() = 0;
            virtual void onTCPConnected() = 0;
            virtual void onTCPSocketClosed() = 0;
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
            static thread::VoidTask<const std::string&, const s32&> connectionTask;
            static ClientListener* listener;
            static Ref<Sender> sender;
            static Ref<Receiver> receiver;
        };
    }

    namespace udp {

        class SenderListener {
        public:
            virtual void onUDPSenderFailed(char* data, size_t size) = 0;
            virtual void onUDPSenderSuccess() = 0;
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
            virtual void onUDPReceiverFailed(char* data, size_t size) = 0;
            virtual void onUDPReceiverSuccess(const YAML::Node& gdNode, const GDHeader& header) = 0;
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
