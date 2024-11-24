/* INCLUDES */
#include <stdio.h>
#include <stdlib.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

/* FUNCTION DECLARATIONS */
extern void* allocate_memory(__uint16_t needed_memory_size);
extern void free_memory(void* chunk);