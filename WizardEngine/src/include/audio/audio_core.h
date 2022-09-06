//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <AL/alc.h>
#include <AL/al.h>
#include <type_traits>

#define alCall(function, ...) alCallImpl(function, __VA_ARGS__)
#define alcCall(function, device, ...) alcCallImpl(function, device, __VA_ARGS__)
#define alHandle(type) (type*) handle

namespace engine::audio {

    bool checkError();

    template<typename alFunction, typename... Params>
    auto alCallImpl(
            alFunction function,
            Params... params
    )->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))> {
        auto result = function(std::forward<Params>(params)...);
#ifdef DEBUG
        checkError();
#endif
        return result;
    }

    template<typename alFunction, typename... Params>
    auto alCallImpl(
            alFunction function,
            Params... params
    )->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool> {
        function(std::forward<Params>(params)...);
        return checkError();
    }

    bool checkError(ALCdevice* device);

    template<typename alcFunction, typename... Params>
    auto alcCallImpl(
            alcFunction function,
            ALCdevice* device,
            Params... params
    )->typename std::enable_if_t<std::is_same_v<void,decltype(function(params...))>,bool> {
        function(std::forward<Params>(params)...);
        return checkError(device);
    }

    template<typename alcFunction, typename ReturnType, typename... Params>
    auto alcCallImpl(
            alcFunction function,
            ReturnType& returnValue,
            ALCdevice* device,
            Params... params
    )->typename std::enable_if_t<!std::is_same_v<void,decltype(function(params...))>,bool> {
        returnValue = function(std::forward<Params>(params)...);
        return checkError(device);
    }

}
