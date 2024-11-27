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
        "a block of memory could not be allocated successfully!"
    );

    /* write into the block of memory */
    memset(block, 0xA, size);

    __uint8_t index;
    for(index = 0; index < size; index++)
    {
        cr_assert(
            eq(i32, ((unsigned char*)block)[index], 0xA), 
            "writing into the memory block was not done successfully"
        );
    }

    /* free the used block of memory */
    free_memory(block);
}


/* test that tries to allocate a block of memory of 0 bytes */
Test(memory_allocation, test_zero_allocation)
{
    void* block = allocate_memory(0);
    cr_assert(eq(block, NULL), "a block of memory of size 0 was allocated!");
}


/* test that checks when a memory block is freed,
if it can be used again afterwards */
Test(memory_allocation, test_simple_free_memory)
{
    void* block = allocate_memory(16);
    cr_assert(
        ne(block, NULL), 
        "block1 was not allocated successfully"
    );

    memory_chunk_t *chunk = (memory_chunk_t *)((char *)block - CHUNK_STRUCT_SIZE);
    cr_assert(
        eq(chunk->metadata.in_use, IN_USE), 
        "returned memory chunk was not set to IN_USE!"
    );

    free_memory(block);
    cr_assert(
        eq(chunk->metadata.in_use, NOT_IN_USE), 
        "memory chunk was not set to NOT_IN_USE!"
    );
}


/* test that tries to free the same block of memory twice */
Test(memory_allocation, test_double_free_memory)
{
    void* block = allocate_memory(16);
    cr_assert(
        ne(block, NULL), 
        "block of memory was not allocated successfully"
    );

    memory_chunk_t *chunk = (memory_chunk_t *)((char *)block - CHUNK_STRUCT_SIZE);
    cr_assert(
        eq(chunk->metadata.in_use, IN_USE), 
        "returned memory chunk was not set to IN_USE!"
    );

    free_memory(block);
    cr_assert(
        eq(chunk->metadata.in_use, NOT_IN_USE), 
        "returned memory chunk was not set to NOT_IN_USE afrer free_memory!"
    );

    free_memory(block);
    cr_assert(
        eq(chunk->metadata.in_use, NOT_IN_USE), 
        "memory chunk was corrupted after second free_memory!"
    );
}


/* test that will check if the memory allocator will allocate the first chunk
that fits exactly or will attempt to split another chunk */
Test(memory_allocation, exact_fit_allocation)
{
    void* block1 = allocate_memory(16);
    cr_assert(
        ne(block1, NULL), 
        "block of memory was not allocated successfully"
    );

    void* block2 = allocate_memory(16);
    cr_assert(
        ne(block2, NULL), 
        "block of memory was not allocated successfully"
    );

    void* block3 = allocate_memory(16);
    cr_assert(
        ne(block3, NULL), 
        "block of memory was not allocated successfully"
    );

    /* in order to not coalesce with adjacent memory chunks, 
    we need the adjacent memory chunks to be in use*/
    free_memory(block2);

    void* block4 = allocate_memory(16);
    cr_assert(
        ne(block4, NULL), 
        "block of memory was not allocated successfully"
    );

    /* check that the new block of memory has the same address as the 
    previous freed block of memory of the same size */
    cr_assert(
        eq(block4, block2),
        "assert that the block of memory received has the same address as the one deallocated recently"
    );

    free_memory(block1);
    free_memory(block3);
    free_memory(block4);
}
