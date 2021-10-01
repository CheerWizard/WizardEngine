//
// Created by mecha on 31.08.2021.
//

#include "LayerStack.h"

namespace engine {

    LayerStack::~LayerStack() {
        for (Layer* layer : _layers) {
            delete layer;
        }
    }

    void LayerStack::pushLayer(Layer* layer) {
        _layers.emplace(_layers.begin() + _layerInsertIndex, layer);
        _layerInsertIndex++;
    }

    void LayerStack::pushOverlay(Layer* overlay) {
        _layers.emplace_back(overlay);
    }

    void LayerStack::popLayer(Layer* layer) {
        auto it = std::find(_layers.begin(), _layers.begin() + _layerInsertIndex, layer);
        if (it != _layers.begin() + _layerInsertIndex) {
            delete layer;
            _layers.erase(it);
            _layerInsertIndex--;
        }
    }

    void LayerStack::popOverlay(Layer* overlay) {
        auto it = std::find(_layers.begin() + _layerInsertIndex, _layers.end(), overlay);
        if (it != _layers.end()) {
            delete overlay;
            _layers.erase(it);
        }
    }

    void LayerStack::onUpdate(Time deltaTime) {
        for (Layer* layer : _layers) {
            layer->onUpdate(deltaTime);
        }
    }

    void LayerStack::onWindowClosed() {
        for (Layer* layer : _layers) {
            layer->onWindowClosed();
        }
    }

    void LayerStack::onWindowResized(unsigned int width, unsigned int height) {
        for (Layer* layer : _layers) {
            layer->onWindowResized(width, height);
        }
    }

    void LayerStack::onKeyPressed(KeyCode keyCode) {
        for (Layer* layer : _layers) {
            layer->onKeyPressed(keyCode);
        }
    }

    void LayerStack::onKeyHold(KeyCode keyCode) {
        for (Layer* layer : _layers) {
            layer->onKeyHold(keyCode);
        }
    }

    void LayerStack::onKeyReleased(KeyCode keyCode) {
        for (Layer* layer : _layers) {
            layer->onKeyReleased(keyCode);
        }
    }

    void LayerStack::onMousePressed(MouseCode mouseCode) {
        for (Layer* layer : _layers) {
            layer->onMousePressed(mouseCode);
        }
    }

    void LayerStack::onMouseRelease(MouseCode mouseCode) {
        for (Layer* layer : _layers) {
            layer->onMouseRelease(mouseCode);
        }
    }

    void LayerStack::onMouseScrolled(double xOffset, double yOffset) {
        for (Layer* layer : _layers) {
            layer->onMouseScrolled(xOffset, yOffset);
        }
    }

    void LayerStack::onCursorMoved(double xPos, double yPos) {
        for (Layer* layer : _layers) {
            layer->onCursorMoved(xPos, yPos);
        }
    }

    void LayerStack::onKeyTyped(KeyCode keyCode) {
        for (Layer* layer : _layers) {
            layer->onKeyTyped(keyCode);
        }
    }

}