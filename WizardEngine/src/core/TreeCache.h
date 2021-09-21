//
// Created by mecha on 19.09.2021.
//

#pragma once

#include "Memory.h"
#include "vector"
#include "unordered_map"

namespace engine {

    template<typename K, typename V>
    class TreeCache {

        typedef std::unordered_map<K, std::vector<Ref<V>>> Tree;
        typedef typename Tree::iterator TreeIterator;

    public:
        TreeCache() = default;
        virtual ~TreeCache() = default;

    public:
        uint32_t add(const K &key, const Ref<V>& value);
        void replace(const K &key, const uint32_t &index, const Ref<V>& value);
        inline const Ref<V>& get(const K &key, const uint32_t &index);
        inline const std::vector<Ref<V>>& get(const K &key);
        void remove(const K &key, const uint32_t &index);
        void remove(const K &key);
        void clear(const K &key);
        void clear();
        bool isEmpty(const K &key);
        inline Ref<V> last(const K &key) const;

    private:
        bool exists(const K &key);

    public:
        TreeIterator begin() {
            return _tree.begin();
        }

        TreeIterator end() {
            return _tree.end();
        }

    private:
         Tree _tree;

    };

}
