#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <type_traits>

namespace tp {

class ThreadPool {
public:
    explicit ThreadPool(size_t thread_count);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // New enqueue that returns a future
    template <typename Func, typename... Args>
    auto enqueue(Func&& func, Args&&... args)
        -> std::future<std::invoke_result_t<Func, Args...>> {

        using return_type = std::invoke_result_t<Func, Args...>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();

        {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.emplace([task]() { (*task)(); });
        }

        cv_.notify_one();
        return res;
    }

private:
    void worker_loop();

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> stop_{false};
};

}

