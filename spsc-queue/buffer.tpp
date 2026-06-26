template <typename T>
SPSC<T>::SPSC(std::size_t capacity) : capacity(capacity) {
    buffer = static_cast<Element*>(malloc(capacity * sizeof(Element)));
    assert(buffer != nullptr);
}

template <typename T>
std::size_t SPSC<T>::increment(std::size_t index) {
    return (index + 1) % capacity;
}

template <typename T>
bool SPSC<T>::try_push(const T& item) {
    std::size_t const currTail = tail.load(std::memory_order_relaxed);
    std::size_t const nextTail = increment(currTail);

    if (nextTail == head.load(std::memory_order_acquire)) {
        return false;
    }

    new (&buffer[currTail]) T(item);
    tail.store(nextTail, std::memory_order_release);
    return true;
}

template <typename T>
bool SPSC<T>::try_pop(auto&& func) {
    std::size_t const currHead = head.load(std::memory_order_relaxed);

    if (currHead == tail.load(std::memory_order_acquire)) {
        return false;
    }

    T& elem = *reinterpret_cast<T*>(&buffer[currHead]);
    func(elem);
    std::destroy_at(&elem);

    head.store(increment(currHead), std::memory_order_release);
    return true;
}

template <typename T>
SPSC<T>::~SPSC() {
    auto noop = [](T&) {};
    while (try_pop(noop));
    free(buffer);
}
