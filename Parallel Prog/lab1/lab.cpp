#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <json/json.h>
#include <fstream>

double process_array(int threads, int* array, int count)
{
    int  max   = -1;                ///< The maximal element

    
    double start = omp_get_wtime();
    /* Find the maximal element */
    #pragma omp parallel num_threads(threads) shared(array, count) reduction(max: max) default(none)
    {
        #pragma omp for
        for(int i=0; i<count; i++)
        {
            if(array[i] > max) { max = array[i]; };
        }
        //printf("-- My lmax is: %d;\n", max);
    }

    float result = omp_get_wtime() - start;
    return result;
}

int main(int argc, char** argv)
{
    const int count = 47483647;     ///< Number of array elements
    int max_threads = 16;         ///< Number of parallel threads to use
    const int random_seed = 920215; ///< RNG seed
    int tests_number = 10;
    int* array;
    int** matrix= (int**)malloc(tests_number*sizeof(int*));
    Json::Value root;
    

    /* Determine the OpenMP support */
    printf("OpenMP: %d;\n", _OPENMP);
    printf("Threads number: %d\n======\n", omp_get_num_procs());
    srand(random_seed);
    for(int i = 0; i<tests_number; i++){
        /* Initialize the RNG */
                /* Generate the random array */
        array = (int*)malloc(count*sizeof(int));
        for(int i=0; i<count; i++) { array[i] = rand(); }
        matrix[i] = array;
    }


    int threads = max_threads;
    while(threads > 0)
    {   
        double sum = 0;
        for(int i = 0; i <tests_number; i++){
            double t = process_array(threads, matrix[i], count);
            sum+=t;
        }
        double averageTime = sum / tests_number;
        root[std::to_string(threads)] = averageTime;
    
        printf("%d threads: %fs\n", threads, averageTime);
        threads--;
    }
    std::ofstream outFile("results.json");
    outFile << root;
    outFile.close();

    return(0);
}