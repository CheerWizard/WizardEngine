//
// Created by mecha on 23.05.2022.
//

#pragma once

#include <yaml/yaml.h>
#include <core/filesystem.h>

namespace engine::io {

    template<class Component>
    class ENGINE_API ComponentSerializer final {

    public:
        ComponentSerializer(const Component& component) : component(component) {}
        ~ComponentSerializer() = default;

    public:
        const char* serializeText();
        void serializeTextFile(const char* filepath);

        void deserializeText(const char* componentText);
        bool deserializeTextFile(const char* filepath);

    private:
        Component component;

    };

    template<class Component>
    const char *ComponentSerializer<Component>::serializeText() {
        YAML::Emitter out;
        component.serialize(out);
        return out.c_str();
    }

    template<class Component>
    void ComponentSerializer<Component>::serializeTextFile(const char *filepath) {
        engine::filesystem::write(filepath, serializeText());
    }

    template<class Component>
    void ComponentSerializer<Component>::deserializeText(const char *componentText) {
        YAML::Node componentNode;
        try {
            componentNode = YAML::Load(componentText);
            component.deserialize(componentNode);
        } catch (YAML::ParserException& e) {
            ENGINE_ERR("ComponentSerializer: Failed to parse YAML text!");
            ENGINE_ERR(e.msg);
            return;
        }
    }

    template<class Component>
    bool ComponentSerializer<Component>::deserializeTextFile(const char *filepath) {
        YAML::Node componentNode;

        try {
            componentNode = YAML::LoadFile(filepath);
            component.deserialize(componentNode);
        } catch (YAML::ParserException& e) {
            ENGINE_ERR("ComponentSerializer: Failed to parse YAML text from '{0}' file!", filepath);
            ENGINE_ERR(e.msg);
            return false;
        } catch (YAML::BadFile& e) {
            ENGINE_ERR("ComponentSerializer: Failed to open '{0}' file", filepath);
            ENGINE_ERR(e.msg);
            return false;
        }

        return true;
    }

}
