//
// Created by mecha on 13.10.2022.
//

#include <scripting/ScriptComponents.h>
#include <scripting/ScriptManager.h>

namespace engine::scripting {

    void CppScript::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "CppScript";
        yaml::serialize(out, "cppName", cppName);
        if (scriptable) {
            scriptable->serialize(out);
        }
        out << YAML::EndMap;
    }

    void CppScript::deserialize(const YAML::Node &parent) {
        auto root = parent["CppScript"];
        if (root) {
            yaml::deserialize(root, "cppName", cppName);
            scriptable = ScriptManager::load(cppName);
            if (scriptable) {
                scriptable->deserialize(root);
            }
        }
    }

}