//
// Created by mecha on 31.08.2021.
//

#include "Layer.h"

void engine::Layer::onCreate() {
    ENGINE_INFO("{0} : onCreate()", tag);
}

void engine::Layer::onUpdate(engine::Time deltaTime) {
    ENGINE_INFO("{0} : onUpdate()", tag);
}

void engine::Layer::onDestroy() {
    ENGINE_INFO("{0} : onDestroy()", tag);
}

void engine::Layer::onImGuiRender() {
    ENGINE_INFO("{0} : onImGuiRender()", tag);
}