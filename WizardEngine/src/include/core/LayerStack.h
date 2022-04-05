//
// Created by mecha on 31.08.2021.
//

#pragma once

#include <core/Layer.h>

#include <vector>
#include <cstdint>

namespace engine::core {

    class LayerStack {

        typedef std::vector<Layer*> Layers;

    public:
        LayerStack() = default;
        ~LayerStack();

    public:
        void onUpdate(time::Time deltaTime);
        void onPrepare();

    public:
        uint32_t pushFront(Layer* layer);
        uint32_t pushBack(Layer* overlay);
        uint32_t popFront(Layer* layer);
        uint32_t popBack(Layer* overlay);

    public:
        void onWindowClosed();
        void onWindowResized(const uint32_t &width, const uint32_t &height);

        void onKeyPressed(event::KeyCode keyCode);
        void onKeyHold(event::KeyCode keyCode);
        void onKeyReleased(event::KeyCode keyCode);
        void onKeyTyped(event::KeyCode keyCode);

        void onMousePressed(event::MouseCode mouseCode);
        void onMouseRelease(event::MouseCode mouseCode);
        void onMouseScrolled(double xOffset, double yOffset);

        void onCursorMoved(double xPos, double yPos);

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