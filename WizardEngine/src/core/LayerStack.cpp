//
// Created by mecha on 31.08.2021.
//

#include "LayerStack.h"

namespace engine {

    LayerStack::~LayerStack() {
        for (Layer* layer : _layers) {
            layer->onDestroy();
            delete layer;
        }
    }

    void LayerStack::pushLayer(Layer* layer) {
        _layers.emplace(_layers.begin() + _layerInsertIndex, layer);
        _layerInsertIndex++;
        layer->onCreate();
    }

    void LayerStack::pushOverlay(Layer* overlay) {
        _layers.emplace_back(overlay);
        overlay->onCreate();
    }

    void LayerStack::popLayer(Layer* layer) {
        auto it = std::find(_layers.begin(), _layers.begin() + _layerInsertIndex, layer);
        if (it != _layers.begin() + _layerInsertIndex) {
            layer->onDestroy();
            _layers.erase(it);
            _layerInsertIndex--;
        }
    }

    void LayerStack::popOverlay(Layer* overlay) {
        auto it = std::find(_layers.begin() + _layerInsertIndex, _layers.end(), overlay);
        if (it != _layers.end()) {
            overlay->onDestroy();
            _layers.erase(it);
        }
    }

    void LayerStack::onUpdate(Time deltaTime) {
        for (Layer* layer : _layers) {
            layer->onUpdate(deltaTime);
        }
    }

}