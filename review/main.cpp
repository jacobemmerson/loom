#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "bank.h"

// 1000 threads deposit $200, 1000 threads withdraw $100 — net effect +$10000
// Final balance should always be $110,000 with correct mutex protection

static const int ITERATIONS = 1000;
static const int OPS_PER_THREAD = 10;

long run_sequential(Account& a) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; i++) {
        a.deposit(200);
        a.withdraw(100, "0000");
    }

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

long run_concurrent(Account& a) {
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS / OPS_PER_THREAD; i++) {
        threads.emplace_back([&a]() {
            for (int j = 0; j < OPS_PER_THREAD; j++) a.deposit(200);
        });
        threads.emplace_back([&a]() {
            for (int j = 0; j < OPS_PER_THREAD; j++) a.withdraw(100, "0000");
        });
    }

    std::cout << "Threads created: " << threads.size() << " | ";

    for (auto& t : threads) t.join();

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

int main() {
    Account seq("John Smith", "0000");
    seq.deposit(10000);
    long seq_time = run_sequential(seq);
    std::cout << "Sequential: " << seq_time << " us | ";
    seq.check_balance("0000");

    Account con("John Smith", "0000");
    con.deposit(10000);
    long con_time = run_concurrent(con);
    std::cout << "Concurrent: " << con_time << " us | ";
    con.check_balance("0000");

    std::cout << "Concurrent was " << (double)con_time / seq_time << "x the sequential time" << std::endl;
}
