# Memory Allocation Library
This repository contains a custom memory allocation library implemented in C. It provides basic dynamic memory management features, such as allocation, deallocation, and memory chunk splitting, using a linked list of memory chunks.

### Features
- Custom Memory Allocation (`allocate_memory`): Allocate memory dynamically using a custom implementation based on a linked list of memory chunks.
- Memory Splitting: Efficiently split larger memory chunks into smaller chunks to fulfill allocation requests.
- Memory Coalescing (`free_memory`): Merge adjacent free memory chunks during deallocation to minimize fragmentation.
- System Memory Management: Use `mmap` to request memory pages from the operating system when needed.
- Metadata Tracking: Each memory chunk contains metadata (size, in-use status) to support dynamic memory operations.

### Key Components
- `allocate_memory(size_t)`: Allocates memory of the requested size and returns a pointer to the usable memory block. Ensures efficient reuse of free chunks or splits existing chunks when possible.
- `free_memory(void*)`: Frees a previously allocated memory block and attempts to coalesce adjacent free blocks to reduce fragmentation.
- `chunk_split(memory_chunk_t*, size_t)`: Splits a larger memory chunk into two smaller chunks, allocating one for the requested size and leaving the remainder available for future allocations.
- `initialize_dynamic_memory()`: Requests a new memory page from the operating system using mmap and initializes it as the first chunk.

### Project Structure
- Source Code:
    + `memory_allocation.c`: Implementation of the memory allocation functions.
    + `memory_allocation.h`: Header file defining the memory_chunk_t struct and function prototypes.
- Tests:
    + `tests.c`: Unit tests implemented using [Criterion](https://github.com/Snaipe/Criterion) to validate the functionality of the memory allocator.

### Usage
1. Include `memory_allocation.h` in your project.
2. Link the library when compiling your project.
3. Use `allocate_memory` and `free_memory` to manage memory dynamically in your application.

### Example
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
- No support for multi-threading; concurrent access may cause undefined behavior.
- Alignment constraints may require modification for specific architectures.
- Assumes a single contiguous memory block per allocation cycle (via mmap).

### License
This project is licensed under the [MIT](https://choosealicense.com/licenses/mit/) License.