//
// Created by mecha on 31.08.2021.
//

#pragma once

#include <vector>

#include "Layer.h"

namespace engine {

    class LayerStack {

        typedef std::vector<Layer*> Layers;

    public:
        LayerStack() = default;
        ~LayerStack();

    public:
        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);
        void popLayer(Layer* layer);
        void popOverlay(Layer* overlay);

    public:
        Layers::iterator begin() { return _layers.begin(); }
        Layers::iterator end() { return _layers.end(); }
        Layers::reverse_iterator rbegin() { return _layers.rbegin(); }
        Layers::reverse_iterator rend() { return _layers.rend(); }

        Layers::const_iterator begin() const { return _layers.begin(); }
        Layers::const_iterator end()	const { return _layers.end(); }
        Layers::const_reverse_iterator rbegin() const { return _layers.rbegin(); }
        Layers::const_reverse_iterator rend() const { return _layers.rend(); }

    private:
        Layers _layers;
        unsigned int _layerInsertIndex = 0;

    };

}