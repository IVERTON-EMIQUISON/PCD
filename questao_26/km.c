#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Usage(char* prog_name);
void Get_args(char* argv[], float* min, float* max, int* data_count, int* thread_count);
void Gen_data(float min, float max, float data[], int data_count);
void Gen_bins(float min, float max, float bin_maxes[], int bin_counts[], int bin_count);
int Which_bin(float value, float bin_maxes[], int bin_count, float min_meas);
void Print_histo(float bin_maxes[], int bin_counts[], int bin_count, float min_meas);

int main(int argc, char* argv[]) {

    int bin_count = 20;
    int i, bin, thread_count;
    float min_meas, max_meas;
    float bin_maxes[20];
    int bin_counts[20];
    int data_count;
    float* data;

    if (argc != 5) Usage(argv[0]);

    Get_args(argv, &min_meas, &max_meas, &data_count, &thread_count);

    data = malloc(data_count * sizeof(float));

    Gen_data(min_meas, max_meas, data, data_count);
    Gen_bins(min_meas, max_meas, bin_maxes, bin_counts, bin_count);

#pragma omp parallel num_threads(thread_count) \
    reduction(+: bin_counts) default(none) \
    shared(data, bin_maxes, data_count, bin_count, min_meas) \
    private(i, bin)
{
#pragma omp for
    for (i = 0; i < data_count; i++) {
        bin = Which_bin(data[i], bin_maxes, bin_count, min_meas);
        bin_counts[bin]++;
    }
}

    Print_histo(bin_maxes, bin_counts, bin_count, min_meas);

    free(data);
    return 0;
}
