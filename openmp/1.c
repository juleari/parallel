#include "omp.h"
#include "rdtsc.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define THREADS 10
#define N 1024
#define M 1024

double arr[N][M];

void init_matrix()
{
    int i, j;

    for ( i = 0; i < N; ++i ) 
        for ( j = 0; j < M; ++j )            
            arr[i][j] = rand()%1000;
}

void divide_row( int row )
{
    int j;
    double divider = arr[row][row];

    for ( j = row; j < M; ++j )
        arr[ row ][ j ] /= divider;
}

int get_max_elem_row( int col )
{
    int i, row = col;
    double  max = fabs( arr[ row ][ col ] ),
            val;

    for ( i = col + 1; i < N; ++i )
    {
        val = fabs( arr[ i ][ col ] );

        if ( val > max )
        {
            max = val;
            row = i;
        }
    }

    return row;
}

void row_sub_row( int row, int i )
{
    int j;
    double divider = arr[i][row];
    
    for ( j = row; j < M; ++j ) 
        arr[i][j] -= arr[row][j] * divider;
}

void swap_rows( int r1, int r2 )
{
    int j;
    double val;

    for ( j = r1; j < M; ++j )
    {
        val = arr[ r1 ][ j ];
        arr[ r1 ][ j ] = arr[ r2 ][ j ];
        arr[ r2 ][ j ] = val;
    }
}

void forward()
{
    int i, j, row_max;

    for ( i = 0; i < N; ++i )
    {
        row_max = get_max_elem_row( i );
        swap_rows( i, row_max );
        divide_row( i );

        #pragma omp parallel for
            for ( j = i + 1; j < N; ++j )
                row_sub_row( i, j );
    }
}

void back()
{
    int i, j;
    for ( j = M - 2; j >= 0; j-- )
    {
        #pragma omp parallel for
            for ( i = 0; i < j; ++i )
            {
                arr[i][M - 1] -= arr[j][M - 1] * arr[i][j];
                arr[i][j] = 0;
            }
    }
}

int main(int argc, char* argv[])
{
    unsigned long long start, finish;
    omp_set_num_threads(THREADS);

    init_matrix();

    start = rdtsc();
    
    forward();
    back();
    
    finish = rdtsc();

    printf("time: %llu\n", finish - start);

    return 0;
}

/*
with:
    time: 2543913706
without:
    time: 3480350707
*/