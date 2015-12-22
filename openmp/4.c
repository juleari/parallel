#include "omp.h"
#include <stdio.h>
#include <stdlib.h>

#define THREADS 8
#define N 1024

int arr[N];

void init_arr()
{
    int i;

    for (i = 0; i < N; ++i)
        arr[i] = rand()%1000;
}

void print_arr()
{
    int i;

    for (i = 0; i < N; ++i)
        printf("%d ", arr[i]);

    printf("\n");
}

int main() 
{
    int i;
    int max;
    omp_set_num_threads(THREADS);
    
    init_arr();
    print_arr();

    max = arr[0];

    #pragma omp parallel for num_threads(4)
        for ( i = 1; i < N; ++i ) 
        {
            #pragma omp critical
            {
                if ( arr[i] > max )
                    max = arr[i];
            }
        }

    printf("max = %d\n", max);

    return 0;
}