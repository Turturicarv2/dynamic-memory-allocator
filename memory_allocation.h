/* INCLUDES */
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdbool.h> 

/* DEFINES */
#define PROT_READ_WRITE (PROT_READ | PROT_WRITE)
#define MAP_PRIVATE_ANONYMOUS (MAP_PRIVATE | MAP_ANONYMOUS)
#define IN_USE 1
#define NOT_IN_USE 0
#define CHUNK_STRUCT_SIZE (sizeof(memory_chunk_t))

/* DEFINED DATA STRUCTURES */
typedef struct chunk_metadata
{
    __uint16_t chunk_size;
    bool in_use;
} chunk_metadata_t;

typedef struct memory_chunk
{
    chunk_metadata_t metadata;
    struct memory_chunk* next_chunk;
} memory_chunk_t;

/* GLOBAL VARIABLE DECLARATIONS */
memory_chunk_t* first_memory_chunk;

/* FUNCTION DECLARATIONS */
void *allocate_memory(__uint16_t needed_memory_size);
void free_memory(void *chunk);
static void initialize_dynamic_memory();
static void *chunk_split(memory_chunk_t *initial_chunk, __uint16_t split_size);
