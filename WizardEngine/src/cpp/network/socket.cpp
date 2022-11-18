//
// Created by vitalii on 30.04.22.
//

#include <platform/platform_detection.h>

#ifdef WINDOWS

#include <network/socket.h>
#include <io/Logger.h>

namespace engine::network::socket {

    bool init() {
        WSAData wsaData;
        WORD version = MAKEWORD(2, 2);
        s32 wsInit = WSAStartup(version, &wsaData);

        if (wsInit != 0) {
            ENGINE_ERR("Unable to initialize Windows socket version: {0}", version);
            return false;
        }

        ENGINE_INFO("Windows socket initialized!");
        return true;
    }

    void cleanup() {
        WSACleanup();
    }

    SOCKET open(const s32& domain, const s32& type, const s32& protocol) {
        return ::socket(domain, type, protocol);
    }

    void close_socket(const SOCKET& socket) {
        closesocket(socket);
    }

    s32 getLastError() {
        return WSAGetLastError();
    }

    s32 connect(const SOCKET& socket, const s32& domain, const char* ip, const s32& port) {
        sockaddr_in hint;
        hint.sin_family = domain;
        hint.sin_port = htons(port);
        inet_pton(domain, ip, &hint.sin_addr);
        // connect to a server
        return ::connect(socket, (sockaddr*)&hint, sizeof(hint));
    }

    s32 bind(const SOCKET& socket, const s32& domain, const s32& port, const s32& inAddressType) {
        sockaddr_in hint;
        hint.sin_family = domain;
        hint.sin_port = htons(port);
        hint.sin_addr.s_addr = inAddressType;
        return ::bind(socket, (sockaddr*)&hint, sizeof(hint));
    }

    s32 listen(const SOCKET& socket, const s32& domain, const s32& port, const s32& inAddressType, const s32& connectionsCount) {
        bind(socket, domain, port, inAddressType);
        return ::listen(socket, connectionsCount);
    }

    SOCKET accept(const SOCKET& socket, sockaddr_in& address) {
        s32 addressSize = sizeof(address);
        return ::accept(socket, (sockaddr*)&address, reinterpret_cast<socklen_t *>(&addressSize));
    }

    SocketProfile getSocketProfile(const SOCKET& socket, sockaddr_in& address) {
        char* host = new char[NI_MAXHOST];
        char* service = new char[NI_MAXSERV];

        memset(host, 0, NI_MAXHOST);
        memset(service, 0, NI_MAXSERV);

        if (getnameinfo((sockaddr*)&address, sizeof(address), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
            ENGINE_INFO("Host connected on port: {0}", service);
        } else {
            inet_ntop(AF_INET, &address.sin_addr, host, NI_MAXHOST);
            ENGINE_INFO("Host connected on port: {0}", ntohs(address.sin_port));
        }

        return { socket, host, service };
    }

    s32 receiveFrom(const SOCKET& socket, char* buffer, size_t size, s32 flags, const sockaddr_in& address) {
        size_t addressSize = sizeof(address);
        return recvfrom(
                socket, buffer, size,
                flags, (sockaddr*)&address,
                reinterpret_cast<socklen_t *>(&addressSize)
        );
    }

    s32 sendTo(const SOCKET& socket, char* buffer, size_t size) {
        return 0;
    }

}

#endif