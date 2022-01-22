//
// Created by mecha on 22.01.2022.
//

#include "WizardEngine.h"

DLL_EXPORT void initLogs(const std::string& engineLog, const std::string& runtimeLog) {
    INIT_ENGINE_LOG(engineLog);
    INIT_RUNTIME_LOG(runtimeLog);
}