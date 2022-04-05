//
// Created by mecha on 31.08.2021.
//

#include <core/LayerStack.h>

namespace engine::core {

    LayerStack::~LayerStack() {
        for (Layer* layer : _layers) {
            delete layer;
        }
    }

    uint32_t LayerStack::pushFront(Layer* layer) {
        _layers.emplace(_layers.begin() + _layerInsertIndex, layer);
        _layerInsertIndex++;
        return _layerInsertIndex - 1;
    }

    uint32_t LayerStack::pushBack(Layer* layer) {
        _layers.emplace_back(layer);
        return 0;
    }

    uint32_t LayerStack::popFront(Layer* layer) {
        auto it = std::find(_layers.begin(), _layers.begin() + _layerInsertIndex, layer);
        if (it != _layers.begin() + _layerInsertIndex) {
            delete layer;
            _layers.erase(it);
            _layerInsertIndex--;
        }
        return _layerInsertIndex;
    }

    uint32_t LayerStack::popBack(Layer* layer) {
        auto it = std::find(_layers.begin() + _layerInsertIndex, _layers.end(), layer);
        if (it != _layers.end()) {
            delete layer;
            _layers.erase(it);
        }
        return 0;
    }

    void LayerStack::onUpdate(time::Time deltaTime) {
        for (Layer* layer : _layers) {
            layer->onUpdate(deltaTime);
        }
    }

    void LayerStack::onWindowClosed() {
        for (Layer* layer : _layers) {
            layer->onWindowClosed();
        }
    }

    void LayerStack::onWindowResized(const uint32_t &width, const uint32_t &height) {
        for (Layer* layer : _layers) {
            layer->onWindowResized(width, height);
        }
    }

    void LayerStack::onKeyPressed(event::KeyCode keyCode) {
        for (Layer* layer : _layers) {
            layer->onKeyPressed(keyCode);
        }
    }

    void LayerStack::onKeyHold(event::KeyCode keyCode) {
        for (Layer* layer : _layers) {
            layer->onKeyHold(keyCode);
        }
    }

    void LayerStack::onKeyReleased(event::KeyCode keyCode) {
        for (Layer* layer : _layers) {
            layer->onKeyReleased(keyCode);
        }
    }

    void LayerStack::onMousePressed(event::MouseCode mouseCode) {
        for (Layer* layer : _layers) {
            layer->onMousePressed(mouseCode);
        }
    }

    void LayerStack::onMouseRelease(event::MouseCode mouseCode) {
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

    void LayerStack::onKeyTyped(event::KeyCode keyCode) {
        for (Layer* layer : _layers) {
            layer->onKeyTyped(keyCode);
        }
    }

    void LayerStack::onPrepare() {
        for (Layer* layer : _layers) {
            layer->onPrepare();
        }
    }

}