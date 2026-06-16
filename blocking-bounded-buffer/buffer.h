#pragma once

#include <mutex>
#include <semaphore>
#include <vector>
#include <string>

static const int MAX_CAP = 1024;

class Buffer {
    private:
        const int capacity;
        std::vector<std::string> contents; // buffer
        std::counting_semaphore<MAX_CAP> empty_slots; // starts full
        std::counting_semaphore<MAX_CAP> full_slots; // start empty
        std::mutex mtx; // mutex

    public:
        Buffer(int capacity);

        void check_contents();

        void produce(std::string x);

        std::string consume();

};