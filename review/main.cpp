#include <iostream>
#include <thread>

// author: @tae
// a simple bank account application to review threads, mutexes, lock_guards, and unique_locks
// compile with g++ -std=c++17 main.cpp -o main


int main(){
    std::thread worker(DoWork);

    std::cin.get();
    s_Finished = true;

    worker.join();

    std::cin.get();

}