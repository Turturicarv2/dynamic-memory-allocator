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