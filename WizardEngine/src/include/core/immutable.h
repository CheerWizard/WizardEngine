//
// Created by mecha on 03.04.2022.
//

#pragma once

// removes copy constructor and assign operator
#define IMMUTABLE(type) \
public: type(const type&) = delete; \
public: type& operator= (const type&) = delete;