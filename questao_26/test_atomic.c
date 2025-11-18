/* File: test_atomic.c
 *
 * Objetivo:
 *   Testar se operações atomic em variáveis diferentes
 *   são tratadas como seções críticas independentes.
 *
 * Como funciona:
 *   - Cada thread tem sua própria variável local (minha_soma)
 *   - Todas executam simultaneamente:
 *
 *       for (i = 0; i < n; i++)
 *           #pragma omp atomic
 *           minha_soma += sin(i);
 *
 *   - Se atomic NÃO criar uma única região crítica global,
 *     tempos com múltiplas threads devem ser parecidos com 1 thread.
 *
 * Compile:
 *   gcc -g -Wall -fopenmp -o test_atomic test_atomic.c
 *
 * Uso:
 *   ./test_atomic <threads>
 *   (entrada padrão: n)
 */

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
    scanf("%lld", &n);

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
            minha_soma += sin(i);
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
