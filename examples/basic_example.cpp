#include <iostream>
#include <thread>
#include "thread_pool/thread_pool.h"

int main() {
    tp::ThreadPool pool(4);

    std::vector<std::future<int>> results;

    for (int i = 0; i < 8; ++i) {
        results.push_back(
            pool.enqueue([i] {
                std::cout << "Task " << i
                          << " running on thread "
                          << std::this_thread::get_id()
                          << std::endl;
                return i * i;
            })
        );
    }

    for (auto& fut : results) {
        std::cout << "Result: " << fut.get() << std::endl;
    }

    return 0;
}
