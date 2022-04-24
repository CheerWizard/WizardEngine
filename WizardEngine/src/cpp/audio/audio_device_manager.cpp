//
// Created by mecha on 23.04.2022.
//

#include <audio/audio_device_manager.h>

namespace engine::audio {

    Ref<Context> DeviceManager::context;
    unordered_map<u8, Ref<Device>> DeviceManager::devices;

    vector<std::string> DeviceManager::getAvailableDevices(void* deviceHandle) {
        const ALCchar* names;
        if (!alcCall(alcGetString, names, (ALCdevice*) deviceHandle, nullptr, ALC_DEVICE_SPECIFIER)) {
            ENGINE_THROW(audio_device_exception("No available devices!"));
        }

        const char* ptr = names;
        vector<std::string> devicesVec;

        do {
            devicesVec.push_back(std::string(ptr));
            ptr += devicesVec.back().size() + 1;
        } while(*(ptr + 1) != '\0');

        return devicesVec;
    }

    u8 DeviceManager::create(const char *name) {
        auto device = createRef<Device>(name);
        devices.insert(std::pair<u8, Ref<Device>>(device->getId(), device));
        return device->getId();
    }

    vector<std::string> DeviceManager::getAvailableDevices(u8 id) {
        return getAvailableDevices(devices[id]->get());
    }

    void DeviceManager::destroy(u8 id) {
        devices.erase(id);
    }

    bool DeviceManager::exists(u8 id) {
        return devices.find(id) != devices.end();
    }

    void DeviceManager::clear() {
        devices.clear();
        context.reset();
    }

    void DeviceManager::createContext(const char *deviceName) {
        u8 deviceId = create(deviceName);
        createContext(deviceId);
    }

    void DeviceManager::createContext(u8 deviceId) {
        context = createRef<Context>(devices[deviceId]);
    }

    const Ref<Device> &DeviceManager::getDevice(u8 id) {
        return devices.at(id);
    }

    const Ref<Context> &DeviceManager::getContext() {
        return context;
    }

}