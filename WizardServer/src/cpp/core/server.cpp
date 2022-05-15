//
// Created by mecha on 15.05.2022.
//

#include <core/server.h>

int main() {
    INIT_ENGINE_LOG("WizardEngine");
    INIT_RUNTIME_LOG("WizardServer");

    ENGINE_INFO("Running WizardServer...");
    network::core::init();
    auto* serverApp = new server::ServerApp();

    while (true) {
        serverApp->update();
    }

    ENGINE_INFO("Shutting down WizardServer...");
    delete serverApp;
    network::core::cleanup();
}

namespace server {

    ServerApp::ServerApp() {
        TCP_SERVER_INIT(this);
        TCP_SERVER_LISTEN_RUN(54000);
        UDP_SERVER_INIT(this);
        UDP_SERVER_BIND_RUN(54000);
    }

    ServerApp::~ServerApp() {
        TCP_SERVER_CLOSE();
        UDP_SERVER_CLOSE();
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

    void ServerApp::udp_socketNotCreated() {

    }

    void ServerApp::udp_socketClosed() {

    }

    void ServerApp::udp_socketBindFailed() {

    }

    void ServerApp::udp_receiveDataFailed(char *data, u16 size) {

    }
}