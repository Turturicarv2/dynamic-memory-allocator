#include "memory_allocation.h"

memory_chunk_t* first_memory_chunk = NULL;

int main() 
{
    int* local_var = (int *)allocate_memory(32);
    *local_var = 2147483647; // MAX_INT
    printf("%d\n", *local_var);

    return 0;
}

void initialize_dynamic_memory()
{
    /* Allocate a large block of memory using mmap */
    void* memory_address = mmap(NULL, 4096, PROT_READ_WRITE, 
            MAP_PRIVATE_ANONYMOUS, -1, 0);
    if(memory_address == (void *) -1)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    /* Initialize first memory chunk */
    first_memory_chunk = (memory_chunk_t*)memory_address;
    first_memory_chunk->metadata.chunk_size = 4096;
    first_memory_chunk->metadata.in_use = false;
    first_memory_chunk->next_chunk = NULL;
}

void* allocate_memory(__uint16_t needed_memory_size)
{
    /* For the first time the function is called, get the memory from the system */
    if(first_memory_chunk == NULL)
    {
        initialize_dynamic_memory();
    }

    memory_chunk_t* current_chunk = first_memory_chunk;
    while(current_chunk != NULL)
    {
        if((current_chunk->metadata.in_use == true)
                || (current_chunk->metadata.chunk_size + CHUNK_STRUCT_SIZE < needed_memory_size))
        {
            current_chunk = current_chunk->next_chunk;
        }
        else if(current_chunk->metadata.chunk_size + CHUNK_STRUCT_SIZE == needed_memory_size)
        {
            current_chunk->metadata.in_use = true;
            return current_chunk;
        }
        else
        {
            return chunk_split(current_chunk, needed_memory_size);
        }
    }

    /* TODO: No memory available case */
}

void* chunk_split(memory_chunk_t* initial_chunk, __uint16_t split_size)
{
    memory_chunk_t* new_memory_chunk = (void *)
            ((char *)initial_chunk + split_size + CHUNK_STRUCT_SIZE);
    new_memory_chunk->metadata.chunk_size = 
            (initial_chunk->metadata.chunk_size - split_size);
    new_memory_chunk->metadata.in_use = false;
    new_memory_chunk->next_chunk = initial_chunk->next_chunk;

    initial_chunk->metadata.chunk_size = split_size;
    initial_chunk->metadata.in_use = true;
    initial_chunk->next_chunk = new_memory_chunk;
    return (void *)((__uint8_t *)initial_chunk + CHUNK_STRUCT_SIZE);
}