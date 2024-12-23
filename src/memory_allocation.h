#ifndef MEMORY_ALLOCATION_H
#define MEMORY_ALLOCATION_H

/* INCLUDES */
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <stdint.h>
#include <pthread.h>

/* DEFINES */
#define PROT_READ_WRITE (PROT_READ | PROT_WRITE)
#define MAP_PRIVATE_ANONYMOUS (MAP_PRIVATE | MAP_ANONYMOUS)
#define IN_USE 1
#define NOT_IN_USE 0
#define CHUNK_STRUCT_SIZE (sizeof(memory_chunk_t))
#define ALLOCATION_SIZE 4096

/* DEFINED DATA STRUCTURES */
typedef struct chunk_metadata
{
    uint16_t chunk_size;
    bool in_use;
} chunk_metadata_t;

typedef struct memory_chunk
{
    chunk_metadata_t metadata;
    struct memory_chunk* next_chunk;
} memory_chunk_t;

/* FUNCTION DECLARATIONS */
void *allocate_memory(__uint16_t needed_memory_size);
void free_memory(void *chunk);

#endif
/* EOF */
