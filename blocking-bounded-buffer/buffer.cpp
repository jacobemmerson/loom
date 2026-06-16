#include "buffer.h"
#include <string>
#include <iostream>

Buffer::Buffer(int capacity) : capacity(capacity), empty_slots(capacity), full_slots(0) {}

void Buffer::check_contents() {
    for (auto& s : contents) {
        std::cout << s << " ";
    }
}

void Buffer::produce(std::string x) {
    
    empty_slots.acquire();
    mtx.lock(); // only allow one producer or consumer at a time
    contents.push_back(x);
    mtx.unlock();
    full_slots.release();

}

std::string Buffer::consume() {
    
    full_slots.acquire();
    mtx.lock(); // only allow one producer or consumer at a time
    std::string out = contents.back();
    contents.pop_back();
    mtx.unlock();
    empty_slots.release();
    return out;
}