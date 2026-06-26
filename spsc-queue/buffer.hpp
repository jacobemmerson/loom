#pragma once

#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <new>

// Lock-free SPSC Queue implementation
// Good for when you need predictable latency without worst-case blocking
// Relies on busy waiting rather than suspending execution
// When used incorrectly, it can be slower than a buffer with a mutex
// Multiple producers / consumers need a mutex!

template <typename T>
class SPSC {
public:
    explicit SPSC(std::size_t capacity);
    ~SPSC();

    bool try_push(const T& item);
    bool try_pop(auto&& func);

private:
    struct Element {
        alignas(T) std::byte storage[sizeof(T)];
    };

    std::size_t increment(std::size_t index);

    std::size_t const capacity;
    std::atomic<std::size_t> head = 0;
    std::atomic<std::size_t> tail = 0;
    Element* buffer;
};

#include "buffer.tpp"
