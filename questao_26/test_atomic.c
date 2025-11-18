/* File: test_atomic.c */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Usage(char* prog_name);

int main(int argc, char* argv[]) {

    int thread_count;
    long long n;

    if (argc != 2) Usage(argv[0]);
    thread_count = strtol(argv[1], NULL, 10);

    printf("Enter n\n");
    if (scanf("%lld", &n) != 1) {
        printf("Erro ao ler n\n");
        exit(1);
    }

    double max_time = 0.0;

#pragma omp parallel num_threads(thread_count) default(none) \
    shared(n, max_time)
    {
        long long i;
        double minha_soma = 0.0;
        double t_start, t_finish;

#pragma omp barrier
        t_start = omp_get_wtime();

        for (i = 0; i < n; i++) {
#pragma omp atomic
            minha_soma += sin((double)i);
        }

        t_finish = omp_get_wtime();

        int tid = omp_get_thread_num();
        printf("Thread %d -> minha_soma = %.4f | tempo = %f s\n",
               tid, minha_soma, t_finish - t_start);

#pragma omp critical
        {
            if (t_finish - t_start > max_time)
                max_time = t_finish - t_start;
        }
    }

    printf("\nTempo final considerado (max thread): %f s\n", max_time);
    return 0;
}

void Usage(char* prog_name) {
    fprintf(stderr, "Uso: %s <threads>\n", prog_name);
    exit(1);
}
