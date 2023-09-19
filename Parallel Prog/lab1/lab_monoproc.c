#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
    const int count = 10000000;     ///< Number of array elements
    const int random_seed = 920215; ///< RNG seed

    int* array = 0;                 ///< The array we need to find the max in
    int  max   = -1;                ///< The maximal element
    clock_t t;

    srand(random_seed);

    /* Generate the random array */
    array = (int*)malloc(count*sizeof(int));
    for(int i=0; i<count; i++) { array[i] = rand(); }

    
    t = clock();
    for(int i=0; i<count; i++)
    {
        if(array[i] > max) { max = array[i]; };
    }

    printf("======\nMax is: %d;\n", max);
    t = clock() - t;
    printf("RUNTIME: %fs\n", ((float)t) / CLOCKS_PER_SEC);
    return(0);
}