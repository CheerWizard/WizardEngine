//
// Created by mecha on 14.01.2022.
//

#pragma once

#include <cstdio>

class Parent {
    public:
        Parent() = default;
        virtual ~Parent() = default;
    public:
        virtual void print(const char* msg);
};