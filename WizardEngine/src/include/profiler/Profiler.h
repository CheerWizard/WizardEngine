//
// Created by mecha on 03.09.2022.
//

#pragma once

#include <time/Timer.h>
#include <core/primitives.h>
#include <fstream>

namespace engine::profiler {

    using namespace core;
    using namespace time;

    struct ProfileData {
        const char* name;
        u32 threadId;
        long long beginTime;
        long long endTime;
    };

    class Profiler final {

    public:
        static void beginSession(const char* filepath);
        static void endSession();
        static void write(const ProfileData& profileData);
        static void onFrameUpdated();
        static bool& isEnabled();
        static u32& getExpectedFrames();

    private:
        static void writeHeader();
        static void writeFooter();

    private:
        static int profileCount;
        static std::ofstream outFile;
        static bool enabled;
        static u32 expectedFrames;
        static u32 actualFrames;
    };

    class ProfileTimer {

    public:
        ProfileTimer(const char* name, float limit = invalid_limit);
        ~ProfileTimer();

    public:
        void stop();

    private:
        const char* name;
        long long beginTime;
        float limit = invalid_limit;
        bool stopped = false;
    };

}

// used for measuring execution time of block of code and writes measured data into JSON file
// then it can be used for web app or soft that can parse this JSON metrics
// for now it's only supported for Chrome Tracing web app inside chrome://tracing
#ifdef PROFILING
#define PROFILE_SESSION_BEGIN(filepath) engine::profiler::Profiler::beginSession(filepath)
#define PROFILE_SESSION_END() engine::profiler::Profiler::endSession()
#define PROFILE_CODE(name, ...) engine::profiler::ProfileTimer timer##__LINE__(name, __VA_ARGS__)
#define PROFILE_FUNCTION(...) PROFILE_CODE(__FUNCSIG__, __VA_ARGS__)
#define PROFILE_ON_FRAME_UPDATED() engine::profiler::Profiler::onFrameUpdated()
#define PROFILE_ENABLED engine::profiler::Profiler::isEnabled()
#define PROFILE_EXPECTED_FRAMES engine::profiler::Profiler::getExpectedFrames()
#else
#define PROFILE_SESSION_BEGIN(filepath)
#define PROFILE_SESSION_END()
#define PROFILE_CODE(name, ...)
#define PROFILE_FUNCTION(...)
#define PROFILE_ON_FRAME_UPDATED()
#define PROFILE_ENABLED
#define PROFILE_EXPECTED_FRAMES
#endif