#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    const int count = 47483647;
    const int random_seed = 920215;
    const int iterations = 10;

    srand(random_seed);

    int max;
    int *array = (int *) malloc(count * sizeof(int));

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int chunk = count / size;

    double start_time, end_time, total = 0;
    for (int iteration = 0; iteration < iterations; iteration++) {
        if (rank == 0) {
            for (int i = 0; i < count; i++) { array[i] = rand(); }
        }

        MPI_Bcast(array, count, MPI_INT, 0, MPI_COMM_WORLD);

        start_time = MPI_Wtime();

        int lmax = array[0];
        for (int i = chunk * rank; i < count && i < chunk * (rank + 1); i++) {
            if (array[i] > lmax) {
                lmax = array[i];
            }
        }

        MPI_Reduce(&lmax, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

        end_time = MPI_Wtime();
        total += end_time - start_time;
    }

    if (rank == 0) {
        printf("\"%d\" : %f,\n", size,(double)  total / (double) iterations);
    }

    MPI_Finalize();

    free(array);
    return 0;
}