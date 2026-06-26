#include "buffer.hpp"
#include "baseline.hpp"
#include <cassert>
#include <nanobench.h>
#include <thread>
#include <iostream>

#ifdef __unix__
#include <pthread.h>
#include <sched.h>

void set_affinity(int cpu) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
}
#endif

void run(auto& buffer)
{
    std::array<std::jthread, 2> threads;

    threads[0] = std::jthread([&] {
        #ifdef __unix__
        set_affinity(10);
        #endif
        for (int i = 0; i < 1'000'000; ++i) {
            while (!buffer.try_push(i)) {
                continue;
            }
        }
    });

    threads[1] = std::jthread([&] {
        #ifdef __unix__
        set_affinity(11);
        #endif
        auto consume = [i = 0](int item) mutable {
            assert(item == i++);
        };
        for (int i = 0; i < 1'000'000; ++i) {
            while (!buffer.try_pop(consume)) {
                continue;
            }
        }
    });
}

int main() {
    ankerl::nanobench::Bench bench;
    bench.minEpochIterations(100).epochs(5).warmup(5);

    SPSC<int> spsc(1000);
    bench.run("spsc (lock-free)", [&spsc]{ run(spsc); });

    MutexQueue<int> mq(1000);
    bench.run("mutex queue (baseline)", [&mq]{ run(mq); });
}
