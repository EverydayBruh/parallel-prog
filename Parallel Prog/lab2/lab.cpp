#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <json/json.h>
#include <fstream>



int* g_default(int count){
    int* array = (int*)malloc(count*sizeof(int));
    for(int i=0; i<count; i++) { array[i] = rand(); }
    return array;
}

int* g_without_target(int count, int element){
    int* array = (int*)malloc(count*sizeof(int));
    for(int i=0; i<count; i++) { 
        array[i] = rand(); 
        if (array[i] == element)
            i--;
    }
    return array;
}

int* g_target_is_first(int count, int element){
    int* array = g_without_target(count, element);
    array[0] = element;
    return array;
}

int* g_target_is_last(int count, int element){
    int* array = g_without_target(count, element);
    array[count] = element;
    return array;
}

int* g_target_in_middle(int count, int element){
    int* array = g_without_target(count, element);
    array[count/2+2] = element;
    return array;
}


int* g_target_everywhere(int count, int element){
    int* array = (int*)malloc(count*sizeof(int));
    for(int i=0; i<count; i++) { array[i] = element; }
    return array;
}



double process_array(int threads, int* array, int count, int const target, int* index)
{
    *index = -1;
    double start = omp_get_wtime();
    /* Find the maximal element */
    #pragma omp parallel num_threads(threads) shared(array, count, target, index) default(none)
    {
        #pragma omp for
        for(int i=0; i<count; i++)
        {
            if(*index != -1)
                i = count;
            if(array[i] == target)
            {
                #pragma omp critical
                *index = i;
            }
        }
        //printf("Found occurence of %d at index %d;\n", target, index);
    }

    float result = omp_get_wtime() - start;
    return result;
}

int main(int argc, char** argv)
{
    const int count = 47483647;     ///< Number of array elements
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
            double t = process_array(threads, matrix[i], count, target, &index);
            sum+=t;
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
