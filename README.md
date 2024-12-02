# Memory Allocation Library

This repository contains a custom memory allocation library implemented in C. It provides thread-safe dynamic memory management features, including allocation, deallocation, memory chunk splitting, and coalescing, using a linked list of memory chunks.

### Features

- Custom Memory Allocation (`allocate_memory`): Allocate memory dynamically using a custom implementation based on a linked list of memory chunks.
- Thread-Safe Memory Management: Ensures safe concurrent access to memory allocation and deallocation functions using a mutex lock (`pthread_mutex_t`).
- Memory Splitting: Efficiently split larger memory chunks into smaller chunks to fulfill allocation requests.
- Memory Coalescing (`free_memory`): Merge adjacent free memory chunks during deallocation to minimize fragmentation.
- System Memory Management: Use `mmap` to request memory pages from the operating system when needed.
- Metadata Tracking: Each memory chunk contains metadata (size, in-use status) to support dynamic memory operations.

### Key Components

- `allocate_memory(size_t)`: Allocates memory of the requested size and returns a pointer to the usable memory block. Ensures efficient reuse of free chunks or splits existing chunks when possible.
- `free_memory(void*)`: Frees a previously allocated memory block and attempts to coalesce adjacent free blocks to reduce fragmentation.
- `chunk_split(memory_chunk_t*, size_t)`: Splits a larger memory chunk into two smaller chunks, allocating one for the requested size and leaving the remainder available for future allocations.
- `initialize_dynamic_memory()`: Requests a new memory page from the operating system using mmap and initializes it as the first chunk.

### Thread Safety

This library supports multi-threading by using a mutex (pthread_mutex_t) to synchronize access to shared memory structures. All critical sections in the allocate_memory and free_memory functions are protected to prevent data races and ensure consistent behavior in concurrent applications.

Key thread-safety mechanisms:
1. Global Mutex Lock: Protects access to the global linked list of memory chunks (`first_memory_chunk`).
2. Error Handling: Ensures the mutex is always unlocked before returning in case of errors or invalid operations.
3. Thread-safe Initialization and Cleanup:
    - Mutex initialization and destruction are managed automatically using constructor and destructor attributes.

### Project Structure

- Source Code:
    + `memory_allocation.c`: Implementation of the memory allocation functions.
    + `memory_allocation.h`: Header file defining the memory_chunk_t struct and function prototypes.
- Tests:
    + `tests.c`: Unit tests implemented using [Criterion](https://github.com/Snaipe/Criterion) to validate the functionality of the memory allocator.
- Makefile:
    + Simplifies building, testing, and cleaning up the project.

### Build and Run Instructions

**Building the Library and Example**

Use the provided `Makefile` for ease of compilation:
+ Build the example program and tests:
```
make
```
+ Build/Run just the example program:
```
make example
```
+ Build/Run the tests:
```  
make tests
```
+ Clean up generated files:
```
make clean
```

### Usage

1. Include memory_allocation.h in your project.
2. Link the library when compiling your project.
3. Use allocate_memory and free_memory to manage memory dynamically.

**Example Code**

```
#include "memory_allocation.h"

int main() {
    // Allocate a block of memory
    void* block = allocate_memory(16);

    // Use the allocated memory
    if (block) {
        memset(block, 0xA, 16);  // Fill memory with 0xA
    }

    // Free the allocated block
    free_memory(block);

    return 0;
}
```

### Known Limitations

- Performance Bottleneck in High-Concurrency Scenarios: A single global mutex may lead to contention in highly concurrent applications.
- Alignment constraints may require modification for specific architectures.
- Assumes a single contiguous memory block per allocation cycle (via mmap).

### License

This project is licensed under the [MIT](https://choosealicense.com/licenses/mit/) License.