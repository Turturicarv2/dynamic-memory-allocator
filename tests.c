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


/* test that will try to push the limits of the memory allocator
1) try 10,000 succeding allocations
2) try maximum number of allocations without freeing */
Test(memory_allocation, stress_test)
{
    uint16_t number_of_allocations = 10000;
    uint8_t size = 16;
    void *block;

    /* Allocate many small blocks consecutively for 10,000 times*/
    for (size_t i = 0; i < number_of_allocations; i++) {
        block = allocate_memory(size);
        cr_assert(
            ne(block, NULL), 
            "block of memory was not allocated successfully"
        );
        free_memory(block);
    }

    uint16_t max_number_allocations = ALLOCATION_SIZE / (size + CHUNK_STRUCT_SIZE);
    void *block_vector[max_number_allocations];

    /* Allocate many small blocks until no more memory available */
    for (size_t i = 0; i < max_number_allocations; i++) {
        block_vector[i] = allocate_memory(size);
        cr_assert(
            ne(block_vector[i], NULL), 
            "block of memory was not allocated successfully"
        );
    }

    /* Try allocating a block of memory when there is no more free memory */
    block = allocate_memory(size);
    cr_assert(
        eq(block, NULL), 
        "block of memory was allocated successfully when there is no memory free!"
    );

    /* Free all memory blocks */
    for (size_t i = 0; i < max_number_allocations; i++) {
        free_memory(block_vector[i]);
    }

    /* Allocate big block of memory */
    block = allocate_memory(ALLOCATION_SIZE - CHUNK_STRUCT_SIZE);
    cr_assert(
        ne(block, NULL), 
        "block of memory was not allocated successfully"
    );
    free_memory(block);
}


/* test the splitting function of the memory allocator.
Split the big memory chunk into 2 smaller chunks. 
First one should be of 128 bytes, 
the other one should have ALLOCATION_SIZE - 128 bytes of memory 
(without considering that CHUNK_STRUCT_SIZE is not counted) */
Test(memory_allocation, splitting_test) {
    /* Allocate a block smaller than the first chunk */
    uint8_t size = 128;
    void *block = allocate_memory(size);
    cr_assert(
        ne(block, NULL), 
        "block of memory was not allocated successfully"
    );

    /* Check if the chunk was split */
    memory_chunk_t *chunk = (memory_chunk_t *)((char *)block - CHUNK_STRUCT_SIZE);
    cr_assert(
        eq(chunk->metadata.in_use, IN_USE), 
        "chunk not marked as in use"
    );
    cr_assert(
        eq(chunk->metadata.chunk_size, size), 
        "chunk size is not correct"
    );

    /* Check the remaining chunk */
    memory_chunk_t *next_chunk = chunk->next_chunk;
    cr_assert(
        ne(next_chunk, NULL), 
        "Next chunk after split is NULL"
    );
    cr_assert(
        eq(next_chunk->metadata.in_use, NOT_IN_USE), 
        "Next chunk should be free"
    );
    cr_assert(
        eq(
            next_chunk->metadata.chunk_size + 2 * CHUNK_STRUCT_SIZE + chunk->metadata.chunk_size,
            ALLOCATION_SIZE
        ),
        "Incorrect split sizes"
    );

    free_memory(chunk);
}

