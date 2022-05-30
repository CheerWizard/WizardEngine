//
// Created by mecha on 15.05.2022.
//

#include <core/server.h>

int main() {
    INIT_ENGINE_LOG("WizardEngine");
    INIT_RUNTIME_LOG("WizardServer");

    RUNTIME_INFO("Running WizardServer...");
    network::socket::init();
    auto* serverApp = new server::ServerApp();

    while (true) {
        serverApp->update();
    }

    RUNTIME_INFO("Shutting down WizardServer...");
    delete serverApp;
    network::socket::cleanup();
}

namespace server {

    ServerApp::ServerApp() {
        bool tcpServerCreated = tcp::Server::init(this);
        if (tcpServerCreated) {
            tcp::Server::listen(54000);
        }

        bool udpServerCreated = udp::Server::init(this);
        if (udpServerCreated) {
            udp::Server::listen(54000);
        }
    }

    ServerApp::~ServerApp() {
        tcp::Server::close();
        udp::Server::close();
    }

    void ServerApp::update() {

    }

    void ServerApp::onTCPSocketCreated() {

    }

    void ServerApp::onTCPSocketUnaccepted() {

    }

    void ServerApp::onTCPSocketClosed() {

    }

    void ServerApp::onTCPDisconnected() {

    }

    void ServerApp::onTCPReceiverFailed(char *data, size_t size) {

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