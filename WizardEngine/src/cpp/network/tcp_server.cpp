//
// Created by mecha on 24.04.2022.
//

#include <network/tcp_server.h>

#include <WS2tcpip.h>
#include <iostream>

#pragma comment (lib, "ws2_32.lib")

namespace engine::network {

    using namespace core;

    decl_exception(socket_exception);

    bool Server::running = false;
    thread::VoidTask<> Server::runTask = {
            "ServerRun_Task",
            "ServerRun_Thread",
            runImpl
    };

    void Server::run() {
        runTask.run();
    }

    void Server::stop() {
        running = false;
    }

    void Server::runImpl() {
        // initialize win socket
        WSAData wsaData;
        WORD version = MAKEWORD(2, 2);
        s32 wsInit = WSAStartup(version, &wsaData);
        if (wsInit != 0) {
            ENGINE_ERR("Unable to initialize win socket version: {0}", version);
            ENGINE_THROW(socket_exception("Win socket error"));
        }

        // create a socket
        SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (listeningSocket == INVALID_SOCKET) {
            ENGINE_THROW(socket_exception("Unable to create LISTENING socket!"));
        }

        // bind IP address amd port to socket
        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(54000);
        hint.sin_addr.S_un.S_addr = INADDR_ANY;
        bind(listeningSocket, (sockaddr*)&hint, sizeof(hint));

        // set the socket for listening
        listen(listeningSocket, SOMAXCONN);

        // wait for connection
        sockaddr_in client;
        s32 clientSize = sizeof(client);
        SOCKET clientSocket = accept(listeningSocket, (sockaddr*)&client, &clientSize);
        if (clientSocket == INVALID_SOCKET) {
            ENGINE_THROW(socket_exception("Unable to create CLIENT socket!"));
        }

        // print client's info
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        ZeroMemory(host, NI_MAXHOST);
        ZeroMemory(service, NI_MAXSERV);

        if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
            ENGINE_INFO("Client host connected on port: {0}", service);
        } else {
            inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
            ENGINE_INFO("Client host connected on port: {0}", ntohs(client.sin_port));
        }

        // close listening socket
        closesocket(listeningSocket);

        // receive message and send back to client
        char buffer[kb_4];
        running = true;
        while (running) {
            ZeroMemory(buffer, kb_4);
            // wait for client to send data
            s32 receivedBytes = recv(clientSocket, buffer, kb_4, 0);
            if (receivedBytes == SOCKET_ERROR) {
                ENGINE_ERR("Server: error during receiving data size: {0}", kb_4);
                break;
            }
            // check client connection
            if (receivedBytes == 0) {
                ENGINE_WARN("Client [host:{0}, service:{1}] disconnected!", host, service);
                break;
            }

            send(clientSocket, buffer, receivedBytes + 1, 0);
        }

        // close client socket
        closesocket(clientSocket);

        // cleanup win socket
        WSACleanup();
    }
}