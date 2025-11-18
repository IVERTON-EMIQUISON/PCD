/* File:    omp_23.c
 *
 * Compile: gcc -g -Wall -fopenmp -o omp_23 omp_23.c
 * Usage:   ./omp_23 <number of threads>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Usage(char* prog_name);

int main(int argc, char* argv[]) {
   int thread_count, thread_number, n;
   int *thread_iteractions;

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   printf("Enter n\n");
   scanf("%d", &n);

  thread_iteractions = (int *)malloc(sizeof(int) * n);

# pragma omp parallel num_threads(thread_count) \
   default(none) private(thread_number) shared(thread_iteractions, n)
   {
#  pragma omp for
      for (int i = 0; i < n; i++) {
         thread_number = omp_get_thread_num();
         thread_iteractions[i] = thread_number;
      }
   }

   for(int k = 0; k < thread_count; k++){
      printf("Thread %d: Iteration ", k);
      for (int i = 0; i < n; i++) {
         if (thread_iteractions[i] == k)
         {
            printf( "%d -- ", i);
         }
      }
      printf("\n");
   }
   
   free(thread_iteractions);
   return 0;
}  /* main */

void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
      exit(0);
}  /* Usage */
