//
// Created by mecha on 17.08.2022.
//

#include <network/NetworkApplication.h>

namespace engine::network {

    void NetworkApplication::onCreate() {
        Application::onCreate();
        socket::init();
    }

    void NetworkApplication::onDestroy() {
        Application::onDestroy();
        socket::cleanup();
    }

}