/* File:    omp_32.c
 *
 * Compile: gcc -g -Wall -fopenmp -o omp_parel_32 omp_parel_32.c
 * Usage:   ./omp_parel_32 <number of threads>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <string.h>

void Usage(char* prog_name);
void Count_sort_parallel(int a[], int n, int thread_count);
void Count_sort(int a[], int n);
int compare(const void* a, const void* b);

int main(int argc, char* argv[]) {
   int thread_count, n, x;
   double start_time, end_time;
   int *a, *b, *c;

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   printf("Enter the amount of data\n");
   scanf("%d", &n);

   a = (int*)malloc(sizeof(int) * n);
   b = (int*)malloc(sizeof(int) * n);
   c = (int*)malloc(sizeof(int) * n);

   for(int i = 0; i < n; i++){
      x = rand();
      a[i] = x;
      b[i] = x;
      c[i] = x;
   }

   start_time = omp_get_wtime();
   Count_sort(a, n);
   end_time = omp_get_wtime();
   printf("Time count sort: %f \n", (end_time - start_time));

   start_time = omp_get_wtime();
   Count_sort_parallel(b, n, thread_count);
   end_time = omp_get_wtime();
   printf("Time count sort parallel: %f \n", (end_time - start_time));

   /*for(int i = 0; i < n; i++){
      printf("%d  -  ", a[i]);
   }*/

   start_time = omp_get_wtime();
   qsort(c, n, sizeof(int), compare);
   end_time = omp_get_wtime();
   printf("Time qsort: %f \n", (end_time - start_time));

   free(a);
   free(b);
   free(c);
   return 0;
}  /* main */

/*-----------------------------------------------------------------------------------------------------------------------*/

void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
      exit(0);
}  /* Usage */

/*-----------------------------------------------------------------------------------------------------------------------*/

void Count_sort(int a[], int n) {
   int i, j, count;
   int* temp = malloc(n*sizeof(int));
   
   for (i = 0; i < n; i++) {
      count = 0;
      for (j = 0; j < n; j++){ 
         if (a[j] < a [i])
            count++;
         else if (a[j] == a[i] && j < i)
            count++;
         }
      temp[count] = a[i];
   }

   memcpy(a, temp, n*sizeof(int));
   free(temp);
} /*Count_sort*/


/*-----------------------------------------------------------------------------------------------------------------------*/

void Count_sort_parallel(int a[], int n, int thread_count) {
int i, j, count;
int *temp = malloc(n*sizeof(int));

#  pragma omp parallel num_threads(thread_count) \
   default (none) shared(temp, a, n, thread_count) private(count, i, j)
   {
#     pragma omp for
      for (i = 0; i < n; i++) {
         count = 0;
         for (j = 0; j < n; j++){
            if (a[j] < a [i])
               count++;
            else if (a[j] == a[i] && j < i)
               count++;
         }
      temp[count] = a[i];
      }

#pragma omp barrier
      int rest = n % thread_count;
      int thread_number = omp_get_thread_num();
      int interval, start;
      if(rest == 0){
         interval = n / thread_count;
         start = thread_number * interval;
      }else{
         if (thread_number < rest){
            interval = (n / thread_count) + 1;
            start = (thread_number * (n/thread_count)) + thread_number;
         }else{
            interval = (n / thread_count);
            start = (thread_number * interval) + rest;
         }
      }

      memcpy(&a[start], &temp[start], interval*sizeof(int));
   }


free(temp);
} /*Count_sort_parallel*/

/*-----------------------------------------------------------------------------------------------------------------------*/

int compare(const void* a, const void* b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    return (int_a > int_b) - (int_a < int_b);
} /*Compare*/

