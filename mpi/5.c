#include "mpi.h"
#include "rdtsc.h"
#include <stdio.h>

int main(int argc, char* argv[]) 
{ 
    int rank, dest, tag;
    char inmsg, outmsg = 'x';
    
    MPI_Status stat;
    MPI_Request req;

    unsigned long long start, finish;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ( rank == 0 )
    {
        dest = 1;
        tag  = 1;
        start = rdtsc();
        
        MPI_Sendrecv
        (
            &outmsg, 1, MPI_CHAR, dest, tag, 
            &inmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &stat
        );
        
        finish = rdtsc();
        printf("0 MPI_Sendrecv time: %llu\n", finish - start);

        tag = 2;
        start = rdtsc();
        
        MPI_Isend
        (
            &outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &req
        );
        MPI_Irecv
        (
            &inmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &req
        );
        
        finish = rdtsc();
        printf("0 MPI_Isend MPI_Irecv time: %llu\n", finish - start);
    }

    if ( rank == 1 )
    {
        dest = 0;
        tag  = 1;
        start = rdtsc();
    
        MPI_Sendrecv
        (
            &outmsg, 1, MPI_CHAR, dest, tag, 
            &inmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &stat
        );
    
        finish = rdtsc();
        printf("1 MPI_Sendrecv time: %llu\n", finish - start);

        tag = 2;
        start = rdtsc();
    
        MPI_Isend
        (
            &outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &req
        );
        MPI_Irecv
        (
            &inmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &req
        );
    
        finish = rdtsc();
        printf("1 MPI_Isend MPI_Irecv time: %llu\n", finish - start);
    }

    MPI_Finalize(); 
   
    return 0; 
}

/*
$ mpirun-mpich-gcc49 -n 4 ./a.out
0 MPI_Sendrecv time: 165170
1 MPI_Sendrecv time: 165800
0 MPI_Isend MPI_Irecv time: 960770
1 MPI_Isend MPI_Irecv time: 966909

$ mpirun-mpich-gcc49 -n 4 ./a.out 
0 MPI_Sendrecv time: 116584
0 MPI_Isend MPI_Irecv time: 26024
1 MPI_Sendrecv time: 117612
1 MPI_Isend MPI_Irecv time: 28633

$ mpirun-mpich-gcc49 -n 4 ./a.out 
0 MPI_Sendrecv time: 127716
0 MPI_Isend MPI_Irecv time: 32392
1 MPI_Sendrecv time: 128580
1 MPI_Isend MPI_Irecv time: 33268

MPI_Isend&MPI_Irecv - работает дольше, чем MPI_Sendrecv.
*/