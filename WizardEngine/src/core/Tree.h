//
// Created by mecha on 19.09.2021.
//

#pragma once

#include "Memory.h"
#include "vector"
#include "unordered_map"

namespace engine {

    template<typename K, typename V>
    class Tree {

        typedef std::unordered_map<K, std::vector<Ref<V>>> MapVector;
        typedef typename MapVector::iterator Iterator;

    public:
        Tree() = default;
        virtual ~Tree() = default;

    public:
        uint32_t add(const K &key, const Ref<V>& value);
        void replace(const K &key, const uint32_t &index, const Ref<V>& value);
        inline const Ref<V>& get(const K &key, const uint32_t &index);
        inline const std::vector<Ref<V>>& get(const K &key);
        inline const Ref<V>& last(const K &key);
        void remove(const K &key, const uint32_t &index);
        void remove(const K &key);
        void clear(const K &key);
        void clear();
        bool isEmpty(const K &key);
        bool isEmpty();

    private:
        bool exists(const K &key);

    public:
        Iterator begin() {
            return _tree.begin();
        }

        Iterator end() {
            return _tree.end();
        }

    private:
         Tree _tree;

    };

    template<typename K, typename V>
    uint32_t Tree<K, V>::add(const K &key, const Ref<V>& value) {
        if (exists(key)) {
            _tree[key].push_back(value);
        } else {
            std::vector<Ref<V>> newVector;
            newVector.push_back(value);
            _tree[key] = newVector;
        }

        return _tree[key].size() - 1;
    }

    template<typename K, typename V>
    void Tree<K, V>::replace(const K &key, const uint32_t &index, const Ref<V>& value) {
        auto vector = _tree[key];
        vector.emplace(vector.begin() + index, value);
    }

    template<typename K, typename V>
    const Ref<V> &Tree<K, V>::get(const K &key, const uint32_t &index) {
        return _tree[key][index];
    }

    template<typename K, typename V>
    void Tree<K, V>::remove(const K &key, const uint32_t &index) {
        auto vector = _tree[key];
        vector.erase(vector.begin() + index);
    }

    template<typename K, typename V>
    void Tree<K, V>::remove(const K &key) {
        clear(key);
        _tree.erase(key);
    }

    template<typename K, typename V>
    void Tree<K, V>::clear(const K &key) {
        if (exists(key)) {
            _tree[key].clear();
        }
    }

    template<typename K, typename V>
    void Tree<K, V>::clear() {
        _tree.clear();
    }

    template<typename K, typename V>
    bool Tree<K, V>::exists(const K &key) {
        return _tree.find(key) != _tree.begin();
    }

    template<typename K, typename V>
    const std::vector<Ref<V>> &Tree<K, V>::get(const K &key) {
        return _tree[key];
    }

    template<typename K, typename V>
    bool Tree<K, V>::isEmpty(const K &key) {
        return get(key).empty();
    }

    template<typename K, typename V>
    const Ref<V>& Tree<K, V>::last(const K &key) {
        return get(key).back();
    }

    template<typename K, typename V>
    bool Tree<K, V>::isEmpty() {
        return _tree.empty();
    }
}
