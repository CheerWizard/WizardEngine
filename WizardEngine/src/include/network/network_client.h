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

    class ENGINE_API RequestQueue final {

    public:
        void push(const NetworkData& networkData);
        template<typename GDBody>
        void push(GDHeader& header, GDBody& body);
        void pop();
        [[nodiscard]] const NetworkData& front() const;
        [[nodiscard]] bool empty() const;

    private:
        queue<NetworkData> _queue;
    };

    template<typename GDBody>
    void RequestQueue::push(GDHeader &header, GDBody &body) {
        push(GDSerializer::serialize(header, body));
    }

    namespace tcp {

        class ENGINE_API SenderListener {
        public:
            virtual void onTCPSenderFailed(char* data, size_t size) = 0;
            virtual void onTCPSenderSuccess() = 0;
        };

        class ENGINE_API Sender final {

        public:
            void run(const SOCKET& clientSocket);
            void stop();
            void close();
            RequestQueue& getRequestQueue();

            inline void setListener(SenderListener* listener) {
                this->listener = listener;
            }

        private:
            void runImpl();

        private:
            SOCKET clientSocket;
            SenderListener* listener = nullptr;
            thread::VoidTask<> senderTask;
            RequestQueue requestQueue;
        };

        class ENGINE_API ReceiverListener {
        public:
            virtual void onTCPReceiverFailed(char* data, size_t size) = 0;
            virtual void onTCPReceiverSuccess(const YAML::Node& gdNode, const GDHeader& header) = 0;
        };

        class ENGINE_API Receiver final {

        public:
            void run(const SOCKET& clientSocket);
            void stop();
            void close();

            inline void setListener(ReceiverListener* listener) {
                this->listener = listener;
            }

        private:
            void runImpl();

        private:
            SOCKET clientSocket;
            ReceiverListener* listener = nullptr;
            thread::VoidTask<> receiverTask;
        };

        class ENGINE_API ClientListener {
        public:
            virtual void onTCPSocketCreated() = 0;
            virtual void onTCPConnectionFailed() = 0;
            virtual void onTCPConnected() = 0;
            virtual void onTCPSocketClosed() = 0;
        };

        class ENGINE_API Client final {

        private:
            Client();

        public:
            static Client& get() {
                static Client instance;
                return instance;
            }

            void close();
            void connect(const std::string& ip, int port);
            RequestQueue& getRequestQueue();

            inline void setListener(ClientListener* listener) {
                this->listener = listener;
            }

            inline void setListener(SenderListener* listener) {
                sender->setListener(listener);
            }

            inline void setListener(ReceiverListener* listener) {
                receiver->setListener(listener);
            }

        private:
            void connectImpl(const std::string& ip, int port);

            SOCKET clientSocket;
            std::thread connectionTask;
            bool connecting = false;
            ClientListener* listener = nullptr;
            Ref<Sender> sender = createRef<Sender>();
            Ref<Receiver> receiver = createRef<Receiver>();
        };
    }

    namespace udp {

        class ENGINE_API SenderListener {
        public:
            virtual void onUDPSenderFailed(char* data, size_t size) = 0;
            virtual void onUDPSenderSuccess() = 0;
        };

        class ENGINE_API Sender final {

        public:
            void run(const SOCKET& clientSocket, const sockaddr_in& server);
            void stop();
            void close();
            RequestQueue& getRequestQueue();

            inline void setListener(SenderListener* listener) {
                this->listener = listener;
            }

        private:
            void runImpl();

        private:
            SOCKET clientSocket;
            sockaddr_in server;
            SenderListener* listener = nullptr;
            thread::VoidTask<> senderTask;
            RequestQueue requestQueue;
        };

        class ENGINE_API ReceiverListener {
        public:
            virtual void onUDPReceiverFailed(char* data, size_t size) = 0;
            virtual void onUDPReceiverSuccess(const YAML::Node& gdNode, const GDHeader& header) = 0;
        };

        class ENGINE_API Receiver final {

        public:
            void run(const SOCKET& clientSocket, const sockaddr_in& server);
            void stop();
            void close();

            inline void setListener(ReceiverListener* listener) {
                this->listener = listener;
            }

        private:
            void runImpl();

        private:
            SOCKET clientSocket;
            sockaddr_in server;
            ReceiverListener* listener = nullptr;
            thread::VoidTask<> receiverTask;
        };

        class ENGINE_API ClientListener {
        public:
            virtual void onUDPSocketCreated() = 0;
            virtual void onUDPSocketClosed() = 0;
        };

        class ENGINE_API Client final {

        private:
            Client();

        public:
            static Client& get() {
                static Client instance;
                return instance;
            }

            inline void setListener(ClientListener* listener) {
                this->listener = listener;
            }

            inline void setListener(SenderListener* listener) {
                sender->setListener(listener);
            }

            inline void setListener(ReceiverListener* listener) {
                receiver->setListener(listener);
            }

            void close();
            void connect(const std::string& ip, int port);
            RequestQueue& getRequestQueue();

        private:
            void connectImpl(const std::string& ip, int port);

        private:
            SOCKET clientSocket;
            sockaddr_in server;
            bool connecting = false;
            std::thread connectionTask;
            ClientListener* listener = nullptr;
            Ref<Sender> sender = createRef<Sender>();
            Ref<Receiver> receiver = createRef<Receiver>();
        };
    }

}
