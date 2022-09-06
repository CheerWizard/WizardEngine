//
// Created by mecha on 03.09.2022.
//

#include <profiler/Profiler.h>
#include <algorithm>
#include <chrono>
#include <thread/Thread.h>

namespace engine::profiler {

    int Profiler::profileCount;
    std::ofstream Profiler::outFile;
    bool Profiler::enabled;
    u32 Profiler::expectedFrames;
    u32 Profiler::actualFrames = 0;

    void Profiler::beginSession(const char *filepath) {
        outFile.open(filepath);
        writeHeader();
    }

    void Profiler::endSession() {
        writeFooter();
        outFile.close();
        profileCount = 0;
    }

    void Profiler::writeHeader() {
        outFile << "{\"otherData\": {},\"traceEvents\":[";
        outFile.flush();
    }

    void Profiler::writeFooter() {
        outFile << "]}";
        outFile.flush();
    }

    void Profiler::write(const ProfileData &profileData) {
        if (profileCount++ > 0) {
            outFile << ",";
        }

        std::string name = profileData.name;
        std::replace(name.begin(), name.end(), '"', '\'');
        long long dt = profileData.endTime - profileData.beginTime;

        outFile << "{";
        outFile << "\"cat\":\"function\",";
        outFile << "\"dur\":" << dt << ',';
        outFile << "\"name\":\"" << name << "\",";
        outFile << "\"ph\":\"X\",";
        outFile << "\"pid\":0,";
        outFile << "\"tid\":" << profileData.threadId << ",";
        outFile << "\"ts\":" << profileData.beginTime;
        outFile << "}";

        outFile.flush();
    }

    bool& Profiler::isEnabled() {
        return enabled;
    }

    void Profiler::onFrameUpdated() {
        if (enabled) {
            enabled = ++actualFrames <= expectedFrames;
        }
        if (!enabled) {
            actualFrames = 0;
        }
    }

    u32 &Profiler::getExpectedFrames() {
        return expectedFrames;
    }

    using namespace std::chrono;

    ProfileTimer::ProfileTimer(const char *name, float limit) : name(name), limit(limit) {
        auto beginTimePoint = high_resolution_clock::now();
        beginTime = time_point_cast<microseconds>(beginTimePoint).time_since_epoch().count();
    }

    ProfileTimer::~ProfileTimer() {
        if (!stopped) {
            stop();
        }
    }

    void ProfileTimer::stop() {
        stopped = true;
        auto endTimePoint = high_resolution_clock::now();
        long long endTime = time_point_cast<microseconds>(endTimePoint).time_since_epoch().count();
        if (Profiler::isEnabled()) {
            Profiler::write({ name, current_thread_id, beginTime, endTime });
        }
    }

}