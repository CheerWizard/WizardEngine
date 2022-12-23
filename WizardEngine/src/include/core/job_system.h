#pragma once

#include <core/core.h>
#include <core/primitives.h>
#include <core/Assert.h>
#include <core/Memory.h>
#include <thread/Thread.h>

#include <functional>
#include <algorithm>
#include <condition_variable>
#include <atomic>
#include <mutex>

namespace engine::core {

    using namespace thread;

    template<typename T>
    class AsyncTask final {
    public:
        AsyncTask() = default;

    public:
        template<typename Function, typename... Args>
        void launch(Function&& f, Args&&... args);
        [[nodiscard]] bool isReady() const;
        T getValue();
        T& getRef();

    private:
        std::launch type = std::launch::async;
        std::future<T> result {};
    };

    template<typename T>
    bool AsyncTask<T>::isReady() const {
        return result.valid();
    }

    template<typename T>
    template<typename Function, typename... Args>
    void AsyncTask<T>::launch(Function&& f, Args&&... args) {
        result = std::async(type, std::forward<Function>(f), std::forward<Args>(args)...);
    }

    template<typename T>
    T AsyncTask<T>::getValue() {
        return result.get();
    }

    template<typename T>
    T& AsyncTask<T>::getRef() {
        return result.get();
    }

    template<typename T, size_t capacity>
    class RingBuffer final {

    public:
        bool pushBack(const T& item);
        bool popFront(T& item);

    private:
        T m_Data[capacity];
        size_t m_Tail = 0;
        size_t m_Head = 0;
        std::mutex m_Lock;
    };

    template<typename T, size_t capacity>
    bool RingBuffer<T, capacity>::pushBack(const T &item) {
        bool pushed = false;
        std::lock_guard<std::mutex> lock(m_Lock);
        size_t next = (m_Head + 1) % capacity;
        if (next != m_Tail) {
            m_Data[m_Head] = item;
            m_Head = next;
            pushed = true;
        }
        return pushed;
    }

    template<typename T, size_t capacity>
    bool RingBuffer<T, capacity>::popFront(T &item) {
        bool popped = false;
        std::lock_guard<std::mutex> lock(m_Lock);
        if (m_Tail != m_Head) {
            item = m_Data[m_Tail];
            m_Tail = (m_Tail + 1) % capacity;
            popped = true;
        }
        return popped;
    }

    struct ENGINE_API JobArgs {
        u32 index = 0;
        u32 groupIndex = 0;
    };

    template<size_t jobs_capacity>
    class JobScheduler final {

    public:
        JobScheduler(u32 workerSize, const ThreadFormat& threadFormat);

    public:
        // executes single job in a single worker thread
        void execute(const std::function<void()>& job);
        // executes multiple jobs with multiple amount of workers per job
        // jobSize - count of inner jobs inside single job
        // jobsPerThread - count of jobs for each worker thread
        void execute(u32 jobsPerThread, u32 jobSize, const std::function<void(JobArgs)>& job);

        inline bool isBusy();

        void wait();

    private:
        // wakes only one worker
        // allows calling thread to be rescheduled by OS
        inline void poll();
        void setupThread(int workerId, const ThreadFormat& threadFormat);

    private:
        RingBuffer<std::function<void()>, jobs_capacity> m_JobPool;
        std::condition_variable m_WakeCondition;
        std::mutex m_WakeMutex;
        u64 m_JobsTodo;
        std::atomic<u64> m_JobsDone;
    };

    template<size_t render_jobs = 32,
            size_t audio_jobs = 32,
            size_t network_jobs = 32,
            size_t thread_pool_jobs = 32>
    class JobSystem final {

    private:
        JobSystem();

    public:
        static JobSystem& get() {
            static JobSystem instance;
            return instance;
        }

        void waitAll();

    public:
        Scope<JobScheduler<render_jobs>> renderScheduler;
        Scope<JobScheduler<audio_jobs>> audioScheduler;
        Scope<JobScheduler<network_jobs>> networkScheduler;
        Scope<JobScheduler<thread_pool_jobs>> threadPoolScheduler;
    };

    template<size_t render_jobs,
            size_t audio_jobs,
            size_t network_jobs,
            size_t thread_pool_jobs>
    JobSystem<render_jobs, audio_jobs, network_jobs, thread_pool_jobs>::JobSystem() {
        // todo this code does not support 1 core CPU.
        const u32 workerSize = std::max(1u, std::thread::hardware_concurrency());
        u32 audioThreads = 1;
        u32 networkThreads = 1;
        u32 renderThreads = 1;
        u32 poolThreads = 1;
        ENGINE_ASSERT(workerSize > 3, "JobSystem(): invalid condition threadPoolSize <= 0");
        renderScheduler = createScope<JobScheduler<render_jobs>>(renderThreads, ThreadFormat(ThreadPriority::HIGHEST, "RenderThread"));
        audioScheduler = createScope<JobScheduler<audio_jobs>>(audioThreads, ThreadFormat(ThreadPriority::HIGHEST, "AudioThread"));
        networkScheduler = createScope<JobScheduler<network_jobs>>(networkThreads, ThreadFormat(ThreadPriority::HIGHEST, "NetworkThread"));
        threadPoolScheduler = createScope<JobScheduler<thread_pool_jobs>>(poolThreads, ThreadFormat(ThreadPriority::NORMAL, "ThreadPoolWorker"));
    }

    template<size_t render_jobs, size_t audio_jobs, size_t network_jobs, size_t thread_pool_jobs>
    void JobSystem<render_jobs, audio_jobs, network_jobs, thread_pool_jobs>::waitAll() {
        renderScheduler->wait();
        audioScheduler->wait();
        networkScheduler->wait();
        threadPoolScheduler->wait();
    }

    template<size_t jobs_capacity>
    JobScheduler<jobs_capacity>::JobScheduler(u32 workerSize, const ThreadFormat& threadFormat) {
        m_JobsTodo = 0;
        m_JobsDone.store(0);
        u32 i = 0;
        while (i < workerSize) {
            setupThread(i++, threadFormat);
        }
    }

    template<size_t jobs_capacity>
    void JobScheduler<jobs_capacity>::execute(const std::function<void()> &job) {
        m_JobsTodo += 1;
        // try to push a new job until it is pushed
        while (!m_JobPool.pushBack(job)) {
            poll();
        }
        m_WakeCondition.notify_one();
    }

    template<size_t jobs_capacity>
    void JobScheduler<jobs_capacity>::execute(u32 jobsPerThread, u32 jobSize, const std::function<void(JobArgs)> &job) {
        if (jobSize == 0 || jobsPerThread == 0) {
            return;
        }

        u32 jobGroups = (jobsPerThread + jobSize - 1) / jobSize;
        m_JobsTodo += jobGroups;
        for (u32 i = 0; i < jobGroups; ++i) {
            // create single job from group
            const auto& jobGroup = [i, job, jobSize, jobsPerThread]() {

                u32 groupJobOffset = i * jobSize;
                u32 groupJobEnd = std::min(groupJobOffset + jobSize, jobsPerThread);

                JobArgs args;
                args.groupIndex = i;

                for (u32 j = groupJobOffset; j < groupJobEnd; ++j) {
                    args.index = j;
                    job(args);
                }
            };
            // try to push new group as a job until its pushed
            while (!m_JobPool.pushBack(jobGroup)) {
                poll();
            }
            m_WakeCondition.notify_one();
        }
    }

    template<size_t jobs_capacity>
    bool JobScheduler<jobs_capacity>::isBusy() {
        return m_JobsDone.load() < m_JobsTodo;
    }

    template<size_t jobs_capacity>
    void JobScheduler<jobs_capacity>::wait() {
        while (isBusy()) {
            poll();
        }
    }

    template<size_t jobs_capacity>
    void JobScheduler<jobs_capacity>::poll() {
        m_WakeCondition.notify_one();
        std::this_thread::yield();
    }

    template<size_t jobs_capacity>
    void JobScheduler<jobs_capacity>::setupThread(int workerId, const ThreadFormat& threadFormat) {
        std::thread worker([this]() {
            std::function<void()> job;
            while (true) {
                if (m_JobPool.popFront(job)) {
                    // execute job and update worker label state
                    job();
                    m_JobsDone.fetch_add(1);
                } else {
                    // no job, put thread to sleep
                    std::unique_lock<std::mutex> lock(m_WakeMutex);
                    m_WakeCondition.wait(lock);
                }
            }
        });
        thread::setThreadFormat(workerId, worker, threadFormat);
        worker.detach();
    }
}

#define RenderScheduler engine::core::JobSystem<>::get().renderScheduler
#define AudioScheduler engine::core::JobSystem<>::get().audioScheduler
#define NetworkScheduler engine::core::JobSystem<>::get().networkScheduler
#define ThreadPoolScheduler engine::core::JobSystem<>::get().threadPoolScheduler
#define waitAllJobs() engine::core::JobSystem<>::get().waitAll()