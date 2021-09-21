//
// Created by mecha on 19.09.2021.
//

#include "TreeCache.h"

namespace engine {

    template<typename K, typename V>
    uint32_t TreeCache<K, V>::add(const K &key, const Ref<V>& value) {
        if (exists(key)) {
            _tree[key].push_back(value);
        } else {
            std::vector<V*> newVector;
            newVector.push_back(value);
            _tree[key] = newVector;
        }
    }

    template<typename K, typename V>
    void TreeCache<K, V>::replace(const K &key, const uint32_t &index, const Ref<V>& value) {
        auto vector = _tree[key];
        vector.emplace(vector.begin() + index, value);
    }

    template<typename K, typename V>
    const Ref<V> &TreeCache<K, V>::get(const K &key, const uint32_t &index) {
        return _tree[key][index];
    }

    template<typename K, typename V>
    void TreeCache<K, V>::remove(const K &key, const uint32_t &index) {
        auto vector = _tree[key];
        vector.erase(vector.begin() + index);
    }

    template<typename K, typename V>
    void TreeCache<K, V>::remove(const K &key) {
        clear(key);
        _tree.erase(key);
    }

    template<typename K, typename V>
    void TreeCache<K, V>::clear(const K &key) {
        if (exists(key)) {
            _tree[key].clear();
        }
    }

    template<typename K, typename V>
    void TreeCache<K, V>::clear() {
        _tree.clear();
    }

    template<typename K, typename V>
    bool TreeCache<K, V>::exists(const K &key) {
        return _tree.find(key) != _tree.begin();
    }

    template<typename K, typename V>
    const std::vector<Ref<V>> &TreeCache<K, V>::get(const K &key) {
        return _tree[key];
    }

    template<typename K, typename V>
    bool TreeCache<K, V>::isEmpty(const K &key) {
        return get(key).empty();
    }

    template<typename K, typename V>
    Ref<V> TreeCache<K, V>::last(const K &key) const {
        return _tree[key].back();
    }

}