#pragma once

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <new>

template <typename T>
class MutexQueue {
public:
    explicit MutexQueue(std::size_t capacity) : capacity(capacity) {
        buffer = static_cast<Element*>(malloc(capacity * sizeof(Element)));
        assert(buffer != nullptr);
    }

    ~MutexQueue() {
        auto noop = [](T&) {};
        while (try_pop(noop));
        free(buffer);
    }

    bool try_push(const T& item) {
        std::unique_lock lock(mutex);

        std::size_t const nextTail = (tail + 1) % capacity;
        if (nextTail == head) return false;

        new (&buffer[tail]) T(item);
        tail = nextTail;
        return true;
    }

    bool try_pop(auto&& func) {
        std::unique_lock lock(mutex);
        if (head == tail) return false;

        T& elem = *reinterpret_cast<T*>(&buffer[head]);
        func(elem);
        std::destroy_at(&elem);

        head = (head + 1) % capacity;
        return true;
    }

private:
    struct Element {
        alignas(T) std::byte storage[sizeof(T)];
    };

    std::size_t capacity;
    std::size_t head = 0;
    std::size_t tail = 0;
    Element* buffer;
    std::mutex mutex;
};
