#include "tests.h"

/* TESTS */
/* simple allocation test that verifies 
writing into the requested block of memory */
Test(memory_allocation, test_simple_allocation)
{
    uint8_t size = 16;
    void* block = allocate_memory(size);
    cr_assert(
        ne(block, NULL), 
        "assert that a block of memory was allocated successfully"
    );

    /* write into the block of memory */
    memset(block, 0xA, size);

    __uint8_t index;
    for(index = 0; index < size; index++)
    {
        cr_assert(
            eq(i32, ((unsigned char*)block)[index], 0xA), 
            "assert that writing into the memory block was done successfully"
        );
    }

    /* free the used block of memory */
    free_memory(block);
}

Test(memory_allocation, test_zero_allocation)
{
    void* block = allocate_memory(0);
    cr_assert(eq(block, NULL), "assert that a block of memory was not allocated");
}

