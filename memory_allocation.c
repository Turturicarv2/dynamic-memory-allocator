/* INCLUDES */
#include "memory_allocation.h"

/* GLOBAL VARIABLIE DEFINITIONS */
memory_chunk_t* first_memory_chunk = NULL;


void* allocate_memory(__uint16_t needed_memory_size)
{
    /* if no memory is needed, then exit function */
    if(needed_memory_size == 0)
    {
        return NULL;
    }

    /* for the first time the function is called, get the memory from the system */
    if(first_memory_chunk == NULL)
    {
        initialize_dynamic_memory();
    }

    /* iterate through the linked list in search for a free chunk */
    memory_chunk_t* current_chunk = first_memory_chunk;
    while(current_chunk != NULL)
    {
        if((current_chunk->metadata.in_use == true)
                || (current_chunk->metadata.chunk_size < needed_memory_size))
        {
            /* go to next memory chunk*/
            current_chunk = current_chunk->next_chunk;
        }
        else if((current_chunk->metadata.chunk_size == needed_memory_size)
                || (current_chunk->metadata.chunk_size - CHUNK_STRUCT_SIZE < needed_memory_size))
        {
            /* use current chunk */
            current_chunk->metadata.in_use = true;
            return (void *)((__uint8_t *)current_chunk + CHUNK_STRUCT_SIZE);
        }
        else
        {
            /* split current chunk */
            return chunk_split(current_chunk, needed_memory_size);
        }
    }

    /* No memory available in linked list */
    return NULL;
}


static void initialize_dynamic_memory()
{
    /* Allocate a large block of memory using mmap */
    void* memory_address = mmap(NULL, 4096, PROT_READ_WRITE, 
            MAP_PRIVATE_ANONYMOUS, -1, 0);
    /* memory allocation failed (no free memory) */
    if(memory_address == MAP_FAILED)
    {
        return;
    }

    /* Initialize first memory chunk */
    first_memory_chunk = (memory_chunk_t*)memory_address;
    first_memory_chunk->metadata.chunk_size = 4096 - CHUNK_STRUCT_SIZE;
    first_memory_chunk->metadata.in_use = false;
    first_memory_chunk->next_chunk = NULL;
}


static void* chunk_split(memory_chunk_t* initial_chunk, __uint16_t split_size)
{
    /* create new memory chunk that will hold the remaining size from the initial chunk */
    memory_chunk_t* new_memory_chunk = (void *)
            ((char *)initial_chunk + split_size + CHUNK_STRUCT_SIZE);
    new_memory_chunk->metadata.chunk_size = 
            (initial_chunk->metadata.chunk_size - split_size - CHUNK_STRUCT_SIZE);
    new_memory_chunk->metadata.in_use = false;
    new_memory_chunk->next_chunk = initial_chunk->next_chunk;

    /* initial chunk will shrink to fit the size needed and get returned */
    initial_chunk->metadata.chunk_size = split_size;
    initial_chunk->metadata.in_use = true;
    initial_chunk->next_chunk = new_memory_chunk;
    return (void *)((char *)initial_chunk + CHUNK_STRUCT_SIZE);
}


void free_memory(void* chunk)
{
    memory_chunk_t* free_chunk = (memory_chunk_t *)((char *)chunk - CHUNK_STRUCT_SIZE);
    free_chunk->metadata.in_use = false;
    
    memory_chunk_t* next_memory_chunk = free_chunk->next_chunk;
    if (next_memory_chunk->metadata.in_use == false) 
    {
        free_chunk->metadata.chunk_size = free_chunk->metadata.chunk_size 
                + next_memory_chunk->metadata.chunk_size + CHUNK_STRUCT_SIZE;
        free_chunk->next_chunk = next_memory_chunk->next_chunk;
    }
}