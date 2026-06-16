# Bounded-Buffer Problem

Also known as the Producer-Consumer Problem, this is a classic concurrency problem that arises when a producer is generating data and a consumer is processing that data. **The data is stored in a shared buffer with a limited capacity**; the buffer is responsible for handling synchronization and communication between the producer and the consumer processes.

Challenges:
- The producer doesn't overwrite existing data in the buffer before it's consumed
- The consumer doesn't read data that has already been processed
- The buffer manages its limited capacity efficiently