/* File:    omp_26.c
 *
 * Compile: gcc -g -Wall -fopenmp -o omp_26 _copia omp_26 _copia.c
 * Usage:   ./omp_26 _copia <number of threads>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Usage(char* prog_name);

int main(int argc, char* argv[]) {

    int num_threads, total_iter;
    double max_thread_time = 0.0;

    if (argc != 2) Usage(argv[0]);
    num_threads = strtol(argv[1], NULL, 10);

    printf("Enter the value of n\n");
    scanf("%d", &total_iter);

#pragma omp parallel num_threads(num_threads) \
    default(none) shared(max_thread_time, total_iter)
    {
        double t_start, t_finish;
        double local_sum = 0.0;
        int i, tid;

#pragma omp barrier
        t_start = omp_get_wtime();

        for (i = 1; i <= total_iter; i++) {

            /* AQUI ESTÁ A DIFERENÇA PRINCIPAL: USAMOS critical NO LUGAR DE atomic */
#pragma omp critical(sum_region)
            local_sum += sin(i);

        }

        tid = omp_get_thread_num();
        printf("Thread %d - local_sum: %.2f\n", tid, local_sum);

        t_finish = omp_get_wtime();
        printf("Thread %d Processing time: %f\n",
               tid, t_finish - t_start);

#pragma omp critical(time_region)
        {
            if (max_thread_time < (t_finish - t_start))
                max_thread_time = (t_finish - t_start);
        }
    }

    printf("Processing time: %f \n", max_thread_time);

    return 0;
}

void Usage(char* prog_name) {
    fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
    exit(0);
}