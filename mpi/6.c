#include "mpi.h"
#include "rdtsc.h"
#include <stdio.h>
#include <stdlib.h>

const int len = 400;

int main(int argc, char* argv[]) 
{ 
    int rank, size, sum = 0, lsum = 0, psum, i, l = len, rsum = 0,
        //**res,
        arr[400];
    MPI_Status stat;
    MPI_Request req;
    unsigned long long start, finish;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (i = 0; i < len; i++) {
        arr[i] = i + 1;
    }

    start = rdtsc();

    lsum = 0;
    for ( i = rank; i < len; i += size )
        lsum += arr[ i ];

    MPI_Reduce(&lsum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    finish = rdtsc();

    if ( rank == 0 )
        printf("MPI_Reduce sum:        %d time: %llu\n", sum, finish - start);

    start = rdtsc();

    lsum = 0;
    for (i = 0; i < l / size; i++)
    {
        lsum += arr[i + (rank * l / size)];
    }

    if ( rank % 2 == 1 ) {
        MPI_Send( &lsum, 1, MPI_INT, rank - 1, rank, MPI_COMM_WORLD );
    }
    else {
        MPI_Recv( &rsum, 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD, &stat );
        lsum += rsum;

        if (rank % 4 != 0) MPI_Send( &lsum, 1, MPI_INT, rank - 2, rank, MPI_COMM_WORLD );
        else 
        {
            MPI_Recv( &rsum, 1, MPI_INT, rank + 2, rank + 2, MPI_COMM_WORLD, &stat );
            lsum += rsum;
        }
    }

    finish = rdtsc();

    if ( rank == 0 )
        printf("MPI_Recv&MPI_Send sum: %d time: %llu\n", lsum, finish - start);
    
    MPI_Finalize(); 
   
    return 0; 
}

/*
$ mpirun-mpich-gcc49 -n 4 ./a.out 
MPI_Reduce sum:        80200 time: 5717536
MPI_Recv&MPI_Send sum: 80200 time: 38755

$ mpirun-mpich-gcc49 -n 4 ./a.out 
MPI_Reduce sum:        80200 time: 189762
MPI_Recv&MPI_Send sum: 80200 time: 86876

$ mpirun-mpich-gcc49 -n 4 ./a.out 
MPI_Reduce sum:        80200 time: 245945
MPI_Recv&MPI_Send sum: 80200 time: 38388
*/