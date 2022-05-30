//
// Created by mecha on 15.05.2022.
//

#include <core/server.h>

int main() {
    INIT_ENGINE_LOG("WizardEngine");
    INIT_RUNTIME_LOG("WizardServer");

    ENGINE_INFO("Running WizardServer...");
    network::socket::init();
    auto* serverApp = new server::ServerApp();

    while (true) {
        serverApp->update();
    }

    ENGINE_INFO("Shutting down WizardServer...");
    delete serverApp;
    network::socket::cleanup();
}

namespace server {

    ServerApp::ServerApp() {
        TCP_SERVER_INIT(this);
        TCP_SERVER_LISTEN_RUN(54000);
        bool udpServerCreated = udp::Server::init(this);
        if (udpServerCreated) {
            udp::Server::listen(54000);
        }
    }

    ServerApp::~ServerApp() {
        TCP_SERVER_CLOSE();
        udp::Server::close();
    }

    void ServerApp::update() {

    }

    void ServerApp::tcp_socketNotCreated() {

    }

    void ServerApp::tcp_clientSocketNotAccepted() {

    }

    void ServerApp::tcp_socketClosed() {

    }

    void ServerApp::tcp_clientDisconnected() {

    }

    void ServerApp::tcp_receiveDataFailed(char *data, u16 size) {

    }

    void ServerApp::onUDPSocketCreated() {

    }

    void ServerApp::onUDPSocketClosed() {

    }

    void ServerApp::onUDPConnectionFailed() {

    }

    void ServerApp::onUDPReceiverFailed(char *data, size_t size) {

    }

    void ServerApp::onUDPSenderFailed(char *data, size_t size) {

    }
}