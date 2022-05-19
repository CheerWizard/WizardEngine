//
// Created by mecha on 25.04.2022.
//

#pragma once

#include <platform/platform_detection.h>
#include <core/primitives.h>

#ifdef WINDOWS
typedef unsigned __int64 SOCKET;
#elseif defined(_WIN32)
typedef _W64 unsigned int SOCKET;
#endif

#ifdef LINUX
typedef s32 SOCKET;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#endif

#define localhost "127.0.0.1"

namespace engine::network::socket {

    using namespace core;
    decl_exception(socket_exception)

    struct SocketProfile {
        SOCKET socket;
        const char* host;
        const char* service;
    };

    void init();
    void cleanup();

    s32 open(const s32& domain, const s32& type, const s32& protocol);
    void close_socket(const SOCKET& socket);

    const char* getLastError();

    s32 connect(const SOCKET& socket, const s32& domain, const char* ip, const s32& port);
    s32 bind(const SOCKET& socket, const s32& domain, const s32& port, const s32& inAddressType);

    void listen(const SOCKET& socket, const s32& domain, const s32& port, const s32& inAddressType, const s32& connectionsCount);
    SOCKET accept(const SOCKET& socket, sockaddr_in& address);

    SocketProfile getSocketProfile(const SOCKET& socket, sockaddr_in& address);

    s32 receiveFrom(const SOCKET& socket, char* buffer, size_t size, s32 flags, const sockaddr_in& address);
    s32 sendTo(const SOCKET& socket, char* buffer, size_t size, );
}