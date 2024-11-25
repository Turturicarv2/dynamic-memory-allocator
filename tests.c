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


/* zero allocation test that tries to allocate a block of memory of 0 bytes */
Test(memory_allocation, test_zero_allocation)
{
    void* block = allocate_memory(0);
    cr_assert(eq(block, NULL), "assert that a block of memory was not allocated");
}


/* simple free memory test that checks that after a memory block is freed,
it can be used again afterwards */
Test(memory_allocation, test_simple_free_memory)
{
    void* block1 = allocate_memory(16);
    cr_assert(
        ne(block1, NULL), 
        "assert that a block of memory was allocated successfully"
    );
    free_memory(block1);

    void* block2 = allocate_memory(16);
    cr_assert(
        ne(block2, NULL), 
        "assert that a block of memory was allocated successfully"
    );
    free_memory(block2);

    cr_assert(
        eq(block1, block2),
        "assert that the second block of memory has the same address as the first one"
    );
}


/* free memory test that tries to free the same block of memory twice */
Test(memory_allocation, test_double_free_memory)
{
    void* block = allocate_memory(16);
    cr_assert(
        ne(block, NULL), 
        "assert that a block of memory was allocated successfully"
    );
    free_memory(block);
    free_memory(block);
}