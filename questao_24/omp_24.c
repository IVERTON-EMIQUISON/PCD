/* File:    omp_24.c
 *
 * Compile: gcc -g -Wall -fopenmp -o omp_24 omp_24.c
 * Usage:   ./omp_24 <number of threads>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Usage(char* prog_name);

int main(int argc, char* argv[]) {
   int thread_count, thread_number, n;
   int *explicit_summation;

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   printf("Enter the value of n\n");
   scanf("%d", &n);

  explicit_summation = (int *)malloc(sizeof(int) * n);

#  pragma omp parallel for num_threads(thread_count) \
      default(none) shared(explicit_summation, n) private(thread_number)
      for (int i = 0; i < n; i++) {
         thread_number = omp_get_thread_num();
         explicit_summation [i] = (i*(i + 1))/2;
         printf("Thread number: %d  -  Index: %d  -  Sum: %d \n", thread_number, i , explicit_summation[i]);      
      }
   
      /*for (int i = 0; i < n; i++) {
         printf("Index: %d  -  Sum: %d \n", i , explicit_summation[i]);
      }*/

   free(explicit_summation);
   return 0;
}  /* main */

void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
      exit(0);
}  /* Usage */
