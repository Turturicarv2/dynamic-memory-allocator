#include "main.h"

int main() 
{
    int* local_var = (int *)allocate_memory(4);
    *local_var = 2147483647; // MAX_INT
    printf("%d\n", *local_var);

    return 0;
}