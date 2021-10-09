//
// Created by mecha on 31.08.2021.
//

#pragma once

#include "Layer.h"

#include <vector>
#include <cstdint>

namespace engine {

    class LayerStack : WindowCallback, KeyboardCallback, MouseCallback, CursorCallback {

        typedef std::vector<Layer*> Layers;

    public:
        LayerStack() = default;
        ~LayerStack();

    public:
        void onUpdate(Time deltaTime);
        void onPrepare();

    public:
        uint32_t pushFront(Layer* layer);
        uint32_t pushBack(Layer* overlay);
        uint32_t popFront(Layer* layer);
        uint32_t popBack(Layer* overlay);

    public:
        void onWindowClosed() override;
        void onWindowResized(const uint32_t &width, const uint32_t &height) override;

        void onKeyPressed(KeyCode keyCode) override;
        void onKeyHold(KeyCode keyCode) override;
        void onKeyReleased(KeyCode keyCode) override;
        void onKeyTyped(KeyCode keyCode) override;

        void onMousePressed(MouseCode mouseCode) override;
        void onMouseRelease(MouseCode mouseCode) override;
        void onMouseScrolled(double xOffset, double yOffset) override;

        void onCursorMoved(double xPos, double yPos) override;

    public:
        typename Layers::iterator begin() { return _layers.begin(); }
        typename Layers::iterator end() { return _layers.end(); }
        typename Layers::reverse_iterator rbegin() { return _layers.rbegin(); }
        typename Layers::reverse_iterator rend() { return _layers.rend(); }

        typename Layers::const_iterator begin() const { return _layers.begin(); }
        typename Layers::const_iterator end()	const { return _layers.end(); }
        typename Layers::const_reverse_iterator rbegin() const { return _layers.rbegin(); }
        typename Layers::const_reverse_iterator rend() const { return _layers.rend(); }

    private:
        Layers _layers;
        uint32_t _layerInsertIndex = 0;

    };

}