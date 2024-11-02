#include "main.h"

int main() 
{
    int* local_var = (int *)allocate_memory(sizeof(int));
    if(local_var == NULL)
    {
        perror("Unable to allocate memory");
        exit(1);
    }
    *local_var = 2147483647; // MAX_INT
    printf("%d\n", *local_var);
    free_memory(local_var);

    return 0;
}