#include "thread_pool/thread_pool.h"

namespace tp {

  ThreadPool::ThreadPool(size_t thread_count) {
    for (size_t i=0; i<thread_count; ++i){
      workers_.emplace_back(&ThreadPool::worker_loop, this);
    }
  }

ThreadPool::~ThreadPool() {
  stop_ = true;
  cv_.notify_all();

  for(auto& t:workers_) {
    if(t.joinable())
      t.join();
  }
}

void ThreadPool::worker_loop() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [&] {
                return stop_ || !tasks_.empty();
            });

            if (stop_ && tasks_.empty())
                return;

            task = std::move(tasks_.front());
            tasks_.pop();
        }

        task();
    }
}

}

#include <future> // needed for std::future, std::packaged_task

namespace tp {

// Template definitions must be in the header.
// So this function should actually go in thread_pool.h
// For simplicity, we will move it there later.

} // namespace tp
