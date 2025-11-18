/* File:    omp_32.c
 *
 * Compile: gcc -g -Wall -fopenmp -o omp_32 omp_32.c
 * Usage:   ./omp_32 <number of threads>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <string.h>

void Usage(char* prog_name);
void Count_sort(int a[], int n);
int compare(const void* a, const void* b);

int main(int argc, char* argv[]) {
   int thread_count, n, number;
   double start_time, end_time;
   int *a, *c;

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   printf("Enter the amount of data\n");
   scanf("%d", &n);

   a = (int*)malloc(sizeof(int) * n);
   c = (int*)malloc(sizeof(int) * n);

   for(int i = 0; i < n; i++){
      number = rand();
      a[i] = number;
      c[i] = number;
   }

   start_time = omp_get_wtime();
   Count_sort(a, n);
   end_time = omp_get_wtime();
   

   /*for(int i = 0; i < n; i++){
      printf("%d  -  ", a[i]);
   }*/

   printf("Total Time: %f", (end_time - start_time));

   start_time = omp_get_wtime();
   qsort(c, n, sizeof(int), compare);
   end_time = omp_get_wtime();

   /*for(int i = 0; i < n; i++){
      printf("%d  -  ", c[i]);
   }*/

   printf("Total Time: %f", (end_time - start_time));

   free(a);
   free(c);
   return 0;
}  /* main */

void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
      exit(0);
}  /* Usage */

void Count_sort(int a[], int n) {
int i, j, count;
int *temp = malloc(n*sizeof(int));
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

int compare(const void* a, const void* b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    return (int_a > int_b) - (int_a < int_b);
} /*Compare*/
