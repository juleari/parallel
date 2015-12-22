#include "mpi.h"
#include "rdtsc.h"
#include <stdio.h>

int main(int argc, char* argv[]) 
{ 
    int size, rank, i;
    MPI_Status Stat;
    char inmsg, outmsg = 'y';
    
    unsigned long long start, finish;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    start = rdtsc();

    MPI_Bcast(&outmsg, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    //printf("Task %d: Received %c from task 0\n", rank, outmsg);

    finish = rdtsc();
    printf("Task %d: MPI_Bcast time: %llu\n", rank, finish - start);

    start = rdtsc();
    
    if ( rank == 0 ) 
    {
        for (i = 1; i < size; i++)
            MPI_Send(&outmsg, 1, MPI_CHAR, i, 1, MPI_COMM_WORLD);
    }
    else 
    {
        MPI_Recv(&inmsg, 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &Stat);
        //printf("Task %d: Received %c from task 0\n", rank, inmsg);
    }

    finish = rdtsc();
    printf("Task %d: MPI_Send&MPI_Recv time: %llu\n", rank, finish - start);

    MPI_Finalize(); 
   
    return 0; 
}

/*
$ mpirun-mpich-gcc49 -n 4 ./a.out 
Task 0: MPI_Bcast time: 131296
Task 0: MPI_Send&MPI_Recv time: 19840
Task 2: MPI_Bcast time: 272744
Task 3: MPI_Bcast time: 414236
Task 3: MPI_Send&MPI_Recv time: 26396
Task 2: MPI_Send&MPI_Recv time: 63048
Task 1: MPI_Bcast time: 201040
Task 1: MPI_Send&MPI_Recv time: 60512

$ mpirun-mpich-gcc49 -n 4 ./a.out 
Task 0: MPI_Bcast time: 95160
Task 0: MPI_Send&MPI_Recv time: 17012
Task 1: MPI_Bcast time: 631877
Task 1: MPI_Send&MPI_Recv time: 30407
Task 2: MPI_Bcast time: 556418
Task 2: MPI_Send&MPI_Recv time: 158108
Task 3: MPI_Bcast time: 863691
Task 3: MPI_Send&MPI_Recv time: 25266

$ mpirun-mpich-gcc49 -n 4 ./a.out 
Task 0: MPI_Bcast time: 103335
Task 0: MPI_Send&MPI_Recv time: 14248
Task 1: MPI_Bcast time: 175089
Task 1: MPI_Send&MPI_Recv time: 26199
Task 2: MPI_Bcast time: 198875
Task 2: MPI_Send&MPI_Recv time: 24021
Task 3: MPI_Bcast time: 238864
Task 3: MPI_Send&MPI_Recv time: 21193
*/