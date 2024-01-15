#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <json/json.h>
#include <fstream>


double shellSort(int* array, int const length)
{
    for (int s  = length /2; s > 0; s/=2) {
        for(int i = 0; i < s; i++) {
            for (int j = i + s; j < length; j+=s) {
                int key = array[j];
                int k = j - s;
                while(k >= 0 && array[k] > key) {
                    array[k + s] = array[k];
                    k -= s;
                }
                array[k + s] = key;
            }
        }
    }
    return 0;
}

void printArray(int* array, int const length){
    printf("\n\n");
    
    for(int i = 0; i < length; i++)
    {
        printf("%d ", array[i]);
    }
}

int main(int argc, char** argv)
{
    const int length = 20;     ///< Number of array elements
    const int random_seed = 920214; ///< RNG seed

    int* array = 0;                 ///< The array we need to sort

    /* Initialize the RNG */
    srand(random_seed);

    /* Generate the random array */
    array = (int*)malloc(length*sizeof(int));
    for(int i=0; i<length; i++) { array[i] = rand(); }

    printArray(array, length);

    shellSort(array, length);

    printArray(array, length);

    return(0);
}