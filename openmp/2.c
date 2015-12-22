#include "omp.h"
#include "rdtsc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THREADS 8
#define N 1024

int arr[N];
int size = N / THREADS;

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

    printf("\n__________\n");
}

int cmp( const void * a, const void * b     )
{
   return ( *(int*)a - *(int*)b );
}

int right( int x )
{
    return ( x + size < N ) ? x : N;
}

void swap_pair( int i, int j )
{
    int val = arr[i];
    arr[ i ] = arr[ j ];
    arr[ j ] = val;
}

void sort_pair( int i )
{
    if ( arr[ i ] > arr[ i + 1 ] )
        swap_pair( i, i + 1 );
}

void merge_blocks(int i) {
    qsort((void*)(arr + (i * size)), right( (i + 1) * size ) - i, sizeof(int), cmp);
}

void easy_sort()
{
    int i, j;

    for (int i = 0; i < N; ++i)
    {
        #pragma omp parallel for
            for ( j = i % 2; j < N; ++j )
                sort_pair( j );
    }
}

void cross_sort()
{
    int i, j;

    #pragma omp parallel for
        for ( i = 0; i < N; i+= size )
            qsort((void*)(arr + i), right( i ) - i, sizeof(int), cmp);

    int k = THREADS;
    while ( k > 1 )
    {
        #pragma omp parallel for
            for ( j = 0; j < k; j += 2 )
                merge_blocks( j );

        k /= 2;
        size *= 2;
    }
}

int main(int argc, char const *argv[])
{
    unsigned long long start, finish;
    omp_set_num_threads(THREADS);
    init_arr();

    start = rdtsc();
    easy_sort();
    finish = rdtsc();
    //print_arr();
    printf("easy:  %llu\n", finish - start);

    init_arr();

    start = rdtsc();
    cross_sort();
    finish = rdtsc();
    //print_arr();
    printf("cross: %llu\n", finish - start);
    return 0;
}

/*
easy:  206905843
cross: 978650
*/