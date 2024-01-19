#include <stdio.h>
#include <omp.h>

int main() {
    fprintf(stdout, "_OPENMP = %d\n", _OPENMP);

    fprintf(stdout, "omp_get_num_procs() = %d\n", omp_get_num_procs());
    fprintf(stdout, "omp_get_max_threads() = %d\n", omp_get_max_threads());

    fprintf(stdout, "omp_get_dynamic() = %d\n", omp_get_dynamic());

    fprintf(stdout, "omp_get_wtick() = %.12f\n", omp_get_wtick());

    fprintf(stdout, "omp_get_nested() = %d\n", omp_get_nested());
    fprintf(stdout, "omp_get_max_active_levels() = %d\n", omp_get_max_active_levels());

    omp_sched_t schedule_kind;
    int chunk_size;

    omp_get_schedule(&schedule_kind, &chunk_size);
    fprintf(stdout, "omp_get_schedule():\nchunk_size = %d\nschedule_kind = %d\n", chunk_size, schedule_kind);
    
    return 0;
}