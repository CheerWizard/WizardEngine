//
// Created by mecha on 23.04.2022.
//

#pragma once

#include <audio/audio_context.h>
#include <core/map.h>

namespace engine::audio {

    class DeviceManager final {

    private:
        DeviceManager() = default;
        ~DeviceManager() = default;

    public:
        static vector<std::string> getAvailableDevices(void* deviceHandle = nullptr);
        static vector<std::string> getAvailableDevices(u8 id);
        static u8 create(const char* name = nullptr);
        static void destroy(u8 id);
        static bool exists(u8 id);
        static void clear();

        static void createContext(const char* deviceName = nullptr);
        static void createContext(u8 deviceId);

        inline static const Ref<Device>& getDevice(u8 id);
        inline static const Ref<Context>& getContext();

    private:
        static Ref<Context> context;
        static unordered_map<u8, Ref<Device>> devices;
    };

}