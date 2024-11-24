#include "tests.h"

Test(memory_allocation, test_simple_allocation)
{
    void* block = allocate_memory(16);
    cr_assert(ne(block, NULL));

    memset(block, 0xA, 16);

    __uint8_t index;
    for(index = 0; index < 16; index++)
    {
        cr_assert(eq(i32, ((unsigned char*)block)[index], 0xA));
    }

    free_memory(block);
}