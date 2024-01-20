#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void gen_array(int* arr, int count) {
  for (int i = 0; i < count; i++) {
    arr[i] = rand() % count;
  }
}

int find_min(int* currentIndices, int* endIndices, int threads, int* arr) {
  int min = -1;
  int j = 0;
  for (int i = 0; i < threads; i++) {
    if ((min == -1 || min > arr[currentIndices[i]]) && currentIndices[i] != endIndices[i]) {
      min = arr[currentIndices[i]];
      j = i;
    }
  }
  currentIndices[j]++;
  return min;
}

double merge(int* sortedSubarrays, int* mergedArray, int count, int threads, int* elementsPerThread, int* startIndices) {
  double start = MPI_Wtime();
  int currentIndices[threads];
  int endIndices[threads];
  for (int i = 0; i < threads; i++) {
    currentIndices[i] = startIndices[i];
    endIndices[i] = currentIndices[i] + elementsPerThread[i];
  }
  endIndices[threads - 1] = count;

  for (int i = 0; i < count; i++) {
    int min = find_min(currentIndices, endIndices, threads, sortedSubarrays);
    mergedArray[i] = min;
  }
  return MPI_Wtime() - start;
}

double shell_sort_mpi(int* array, int count) {
  int step;
  MPI_Barrier(MPI_COMM_WORLD);
  double local_start = MPI_Wtime();
  for (step = count / 2; step > 0; step /= 2) {
    {
      for (int k = 0; k < step; k++) {
        for (int i = k; i < count; i += step) {
          int tmp, j;
          tmp = array[i];
          for (j = i; j >= step; j -= step) {
            if (tmp < array[j - step])
              array[j] = array[j - step];
            else
              break;
          }
          array[j] = tmp;
        }
      }
    }
  }
  double local_time = MPI_Wtime() - local_start;
  double global_time;
  MPI_Reduce(&local_time, &global_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  return global_time;
}


void fill_counts(int* elementsPerThread, int* startIndices, int threads, int count) {
  int part_size = count / threads;
  for (int i = 0; i < threads; i++) {
    startIndices[i] = i*part_size;
    elementsPerThread[i] = part_size;
  }
  elementsPerThread[threads - 1] = count - part_size*(threads - 1);
}

void stress(int* arr, int count, int test_count, int argc, char** argv) {
  int rank, size;
  int *res = calloc(count, sizeof(int));

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int elementsPerThread[size];
  int startIndices[size]; 
  fill_counts(elementsPerThread, startIndices, size, count);

  double total_time = 0;

  for (int i = 0; i < test_count; i++) {
    if (rank == 0) {
      gen_array(arr, count);
    }

    MPI_Scatterv(arr, elementsPerThread, startIndices, MPI_INT, res, elementsPerThread[rank], MPI_INT, 0, MPI_COMM_WORLD);
    total_time += shell_sort_mpi(res, elementsPerThread[rank]);
    MPI_Gatherv(res, elementsPerThread[rank], MPI_INT, arr, elementsPerThread, startIndices, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
      total_time += merge(arr, res, count, size, elementsPerThread, startIndices);
    }

  }
    
  if (rank == 0) {
    printf("\"%d\" : %lf,\n", size,(double)  total_time / (double) test_count);
  }
  free(res);
}

int main(int argc, char **argv) {
  int mass_count = 1e7;
  if (argc > 1) {
    mass_count = atoi(argv[1]);
  }
  int test_count = 10;
  if (argc > 2) {
    test_count = atoi(argv[2]);
  }
  int* array = calloc(sizeof(int), mass_count);

  MPI_Init(&argc, &argv);  

  stress(array, mass_count, test_count, argc, argv);

  MPI_Finalize();

  free(array);
  return 0;
}
