#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <json/json.h>
#include <fstream>


double shellSort(int* array, int const count, int threads)
{
    double start = omp_get_wtime();
    for (int s  = count /2; s > 0; s/=2) {
        #pragma omp parallel num_threads(threads) shared(array, count, s) 
        {
            #pragma omp for
            for(int i = 0; i < s; i++) {
                for (int j = i + s; j < count; j+=s) {
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
    }
    float result = omp_get_wtime() - start;
    return result;
}


int* g_default(int count){
    int* array = (int*)malloc(count*sizeof(int));
    for(int i=0; i<count; i++) { array[i] = rand(); }
    return array;
}

void printArray(int* array, int const length){
    printf("\n\n");
    
    for(int i = 0; i < length; i++)
    {
        printf("%d ", array[i]);
    }
}

int* copy_array(int* array, int const count){
    int* result = (int*)malloc(count*sizeof(int));
    for(int i=0; i<count; i++) { result[i] = array[i]; }
    return result;
}

int main(int argc, char** argv)
{
    const int count = 1e7;     ///< Number of array elements
    int max_threads = 16;         ///< Number of parallel threads to use
    int target = 16;
    const int random_seed = 920215; ///< RNG seed
    int tests_number = 10;
    int index;
    int* array;
    int** matrix= (int**)malloc(tests_number*sizeof(int*));
    Json::Value root;
    

    /* Determine the OpenMP support */
    printf("OpenMP: %d;\n", _OPENMP);
    printf("Threads number: %d\n======\n", omp_get_num_procs());
    srand(random_seed);
    for(int i = 0; i<tests_number; i++){
        array = g_default(count);
        matrix[i] = array;
    }


    int threads = 1;
    while(threads <= max_threads)
    {   
        double sum = 0;
        for(int i = 0; i <tests_number; i++){
            array = copy_array(matrix[i], count);
            double t = shellSort(array, count, threads);
            sum+=t;
            free(array);
        }
        double averageTime = sum / tests_number;
        root[std::to_string(threads)] = averageTime;    //save result in json
    
        printf("%d threads: %fs\n", threads, averageTime);
        threads++;
    }
    std::ofstream outFile("results.json");
    outFile << root;
    outFile.close();

    return(0);
}