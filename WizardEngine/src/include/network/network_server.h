//
// Created by mecha on 24.04.2022.
//

#pragma once

#include <thread/Task.h>
#include <network/socket.h>
#include <network/gdp.h>

namespace engine::network {

    using namespace engine::core;

    struct ENGINE_API ClientProfile {
        SOCKET socket;
        const char* host;
        const char* service;
    };

    class ENGINE_API SceneService {

    public:
        void dispatch(const YAML::Node& gdNode, const GDHeader& header);

    protected:
        virtual void send(char* data, size_t size) = 0;

    private:
        void saveScene(const YAML::Node &gdNode);
        void loadScene(const YAML::Node &gdNode);
    };

    namespace tcp {

        class ENGINE_API ServerListener {
        public:
            virtual void onTCPSocketCreated() = 0;
            virtual void onTCPSocketUnaccepted() = 0;
            virtual void onTCPSocketClosed() = 0;
            virtual void onTCPDisconnected() = 0;
            virtual void onTCPReceiverFailed(char* data, size_t size) = 0;
        };

        class ENGINE_API TCPSceneService : public SceneService {

        public:
            void init(SOCKET socket);

        protected:
            void send(char *data, size_t size) override;

        private:
            SOCKET socket{};
        };

        class ENGINE_API Server final {

        public:
            static bool init(ServerListener* serverListener);
            static void close();

            static void listen(const s32& port);
            static void stop();

        private:
            static void listenImpl(const s32& port);
            static void runImpl();

        private:
            static SOCKET listeningSocket;
            static ClientProfile clientProfile;
            static thread::VoidTask<const s32&> listenTask;
            static ServerListener* listener;
            static TCPSceneService sceneService;
        };
    }

    namespace udp {

        class ENGINE_API ServerListener {
        public:
            virtual void onUDPSocketCreated() = 0;
            virtual void onUDPSocketClosed() = 0;
            virtual void onUDPConnectionFailed() = 0;
            virtual void onUDPReceiverFailed(char* data, size_t size) = 0;
            virtual void onUDPSenderFailed(char* data, size_t size) = 0;
        };

        class ENGINE_API UDPSceneService : public SceneService {

        public:
            void init(SOCKET socket, const sockaddr_in& hint);

        protected:
            void send(char *data, size_t size) override;

        private:
            SOCKET socket{};
            sockaddr_in hint;
        };

        class ENGINE_API Server final {

        private:
            Server() = default;
            ~Server() = default;

        public:
            static bool init(ServerListener* serverListener);
            static void close();

            static void listen(const s32& port);
            static void stop();

        private:
            static void listenImpl(const s32& port);
            static void runImpl();

            static void send(char* data, size_t size);

        private:
            static SOCKET clientSocket;
            static thread::VoidTask<const s32&> listenTask;
            static ServerListener* listener;
            static sockaddr_in client;
            static UDPSceneService sceneService;
        };

    }

}
