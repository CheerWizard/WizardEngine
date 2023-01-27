#include <thread/Thread.h>
#include <platform/platform_detection.h>

#include <thread>
#include <sstream>

#ifdef WINDOWS
#define NOMINMAX
#include <Windows.h>
#endif

namespace engine::thread {

    void current_sleep(const uint32_t &millis) {
        std::this_thread::sleep_for(std::chrono::milliseconds(millis));
    }

    u32 cpu_cores_count() {
        return std::thread::hardware_concurrency();
    }

    u32 currentThreadId() {
        u32 threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
        return threadId;
    }

    void setThreadFormat(u32 id, std::thread& thread, const ThreadFormat& format) {

#if defined(WINDOWS)

        HANDLE handle = (HANDLE)thread.native_handle();
        // set thread into dedicated core
        DWORD_PTR affinityMask = 1ull << id;
        DWORD_PTR affinity_result = SetThreadAffinityMask(handle, affinityMask);
        assert(affinity_result > 0);
        // set priority
        int priority = THREAD_PRIORITY_NORMAL;
        switch (format.priority) {
            case ThreadPriority::LOWEST:
                priority = THREAD_PRIORITY_LOWEST;
                break;
            case ThreadPriority::NORMAL:
                priority = THREAD_PRIORITY_NORMAL;
                break;
            case ThreadPriority::HIGHEST:
                priority = THREAD_PRIORITY_HIGHEST;
                break;
        }
        BOOL priority_result = SetThreadPriority(handle, priority);
        assert(priority_result != 0);
        // set thread name
        std::wstringstream wss;
        wss << format.name << "-" << id;
        HRESULT hr = SetThreadDescription(handle, wss.str().c_str());
        assert(SUCCEEDED(hr));

#elif defined(LINUX)

        #define handle_error_en(en, msg) \
               do { errno = en; perror(msg); } while (0)

			int ret;
			cpu_set_t cpuset;
			CPU_ZERO(&cpuset);
			size_t cpusetsize = sizeof(cpuset);

			CPU_SET(threadID, &cpuset);
			ret = pthread_setaffinity_np(worker.native_handle(), cpusetsize, &cpuset);
			if (ret != 0)
				handle_error_en(ret, std::string(" pthread_setaffinity_np[" + std::to_string(threadID) + ']').c_str());

			// Name the thread
			std::string thread_name = "wi::job::" + std::to_string(threadID);
			ret = pthread_setname_np(worker.native_handle(), thread_name.c_str());
			if (ret != 0)
				handle_error_en(ret, std::string(" pthread_setname_np[" + std::to_string(threadID) + ']').c_str());
#undef handle_error_en

#endif
    }
}
