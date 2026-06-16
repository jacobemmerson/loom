#include "buffer.h"
#include <iostream>
#include <thread>
#include <vector>
#include <string>

// compile with g++ -std=c++20 -pthread main.cpp buffer.cpp -o buffer && ./buffer

static std::mutex print_mtx;

void log(const std::string& msg) {
    std::lock_guard<std::mutex> lock(print_mtx);
    std::cout << msg << "\n";
}

void producer(Buffer& buf, int id, int num_items) {
    for (int i = 0; i < num_items; i++) {
        std::string item = "P" + std::to_string(id) + "-item" + std::to_string(i);
        buf.produce(item);
        log("[producer " + std::to_string(id) + "] produced: " + item);
    }
}

void consumer(Buffer& buf, int id, int num_items) {
    for (int i = 0; i < num_items; i++) {
        std::string item = buf.consume();
        log("[consumer " + std::to_string(id) + "] consumed: " + item);
    }
}

int main() {
    const int BUFFER_CAP = 3;   // small buffer to make blocking visible
    const int NUM_PRODUCERS = 2;
    const int NUM_CONSUMERS = 2;
    const int ITEMS_PER_THREAD = 4;

    Buffer buf(BUFFER_CAP);

    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_PRODUCERS; i++)
        threads.emplace_back(producer, std::ref(buf), i, ITEMS_PER_THREAD);

    for (int i = 0; i < NUM_CONSUMERS; i++)
        threads.emplace_back(consumer, std::ref(buf), i, ITEMS_PER_THREAD);

    for (auto& t : threads)
        t.join();

    std::cout << "\nFinal buffer contents: ";
    buf.check_contents();
    std::cout << "(empty)\n";
}
