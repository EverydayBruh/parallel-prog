#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int my_sqrt(int x) {
  int startRange = 1;
  int endRange = x;
  while (endRange - startRange > 1) {
    long long m = (endRange + startRange) / 2;
    if (m*m > x) {
      endRange = m;
    } else {
      startRange = m;
    }
  }
  return startRange;
}

int is_prime_omp(int x, int threads) {
  int sqr = my_sqrt(x);
  int res = 1;
  #pragma omp parallel num_threads(threads) shared(res, sqr, x) default(none)
  {
    #pragma omp for
    for (int i = 2; i < sqr; i++) {
      if (x % i == 0) {
        res = 0;
      }
      if (res == 0) {
        i = sqr;
      }
    }
  }
  return res;
}


double find_primes_mpi(int startRange, int endRange, int threads, int* res) {
  MPI_Barrier(MPI_COMM_WORLD);
  double local_start = MPI_Wtime();
  int local_res = 0;

  for (int i = startRange; i < endRange; i++)
    local_res += is_prime_omp(i, threads);
    // local_res += is_prime_wo_omp(i);
  
  double local_time = MPI_Wtime() - local_start;
  MPI_Reduce(&local_res, res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  return local_time;
}


void stress(int max_threads, int sect_size, int test_count) {
  int rank, size;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double local_time = 0;
  int startRange;
  int endRange;

  for (int i = 0; i < test_count; i++) {
    if (rank == 0) {
      srand(i + 50);
      startRange = rand() - sect_size;
    }
    int primeCount = 0;

    MPI_Bcast(&startRange, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int thread_sect_size = sect_size / size;
    endRange = (rank == size - 1) ? startRange + sect_size 
                                  : startRange + thread_sect_size * (rank + 1);
    startRange += rank * thread_sect_size;
    local_time += find_primes_mpi(startRange, endRange, max_threads/size, &primeCount);

  }
  printf("\"%d\" : %lf,\n", rank, local_time / test_count);
  
}

int main(int argc, char **argv) {
  int size = 1e5;
  if (argc > 1) {
    size = atoi(argv[1]);
  }
  int test_count = 5;
  int max_threads = 16;
  if (argc > 2) {
    max_threads = atoi(argv[2]);
  }

  MPI_Init(&argc, &argv);  

  stress(max_threads, size, test_count);

  MPI_Finalize();

  return 0;
}
