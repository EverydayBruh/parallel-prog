#include <stdio.h>
#include <omp.h>

#define ARRAY_SIZE 10
#define NUM_THREADS 10

int main() {
    int sum = 0;
    int data[ARRAY_SIZE];

    // Инициализация массива
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        data[i] = i + 1;
    }

    omp_lock_t lock;
    omp_init_lock(&lock);

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int local_sum = 0;

        #pragma omp for
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            local_sum += data[i];
        }

        // Явная блокировка перед обновлением общей переменной
        omp_set_lock(&lock);
        sum += local_sum;
        printf("Sum: %d\n", sum);
        omp_unset_lock(&lock);
    }

    // Освобождение блокировки
    omp_destroy_lock(&lock);

    printf("Total sum: %d\n", sum);

    return 0;
}