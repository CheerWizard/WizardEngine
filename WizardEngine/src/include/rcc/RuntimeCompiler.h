//
// Created by mecha on 06.09.2022.
//

#pragma once

#include <RuntimeObjectSystem.h>
#include <ICompilerLogger.h>
#include <time/Time.h>

namespace engine::rcc {

    struct ENGINE_API RCCLogger : ICompilerLogger {
        ~RCCLogger() override = default;

        void LogError(const char *format, ...) override;
        void LogWarning(const char *format, ...) override;
        void LogInfo(const char *format, ...) override;
    };

    class ENGINE_API RuntimeCompiler final {

    public:
        static bool init();
        static void release();
        static void onUpdate(time::Time dt);

    private:
        static IRuntimeObjectSystem* ros;
        static RCCLogger logger;
    };

}
