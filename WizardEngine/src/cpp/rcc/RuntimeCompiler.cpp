//
// Created by mecha on 06.09.2022.
//

#include <rcc/RuntimeCompiler.h>

namespace engine::rcc {

    IRuntimeObjectSystem* RuntimeCompiler::ros;
    RCCLogger RuntimeCompiler::logger;

    bool RuntimeCompiler::init() {
        ros = new RuntimeObjectSystem();
        if (!ros->Initialise(&logger, nullptr)) {
            ENGINE_ERR("RuntimeCompiler: failed to initialize!");
            delete ros;
            ros = nullptr;
            return false;
        }
        ENGINE_INFO("RuntimeCompiler: initialized!");
        return true;
    }

    void RuntimeCompiler::release() {
        delete ros;
    }

    void RuntimeCompiler::onUpdate(time::Time dt) {
        if (ros->GetIsCompiledComplete()) {
            ros->LoadCompiledModule();
        }
        if (!ros->GetIsCompiling()) {
            ros->GetFileChangeNotifier()->Update(dt.milliseconds());
        }
    }

    void RCCLogger::LogError(const char *format, ...) {
        ENGINE_ERR(format);
    }

    void RCCLogger::LogWarning(const char *format, ...) {
        ENGINE_WARN(format);
    }

    void RCCLogger::LogInfo(const char *format, ...) {
        ENGINE_INFO(format);
    }

}