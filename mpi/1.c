#include "mpi.h"
#include <stdio.h>

int main(int argc, char* argv[]) 
{ 
    int procs_count, world_rank, new_rank, *ranks, i;
    MPI_Group group, new_group;
    MPI_Comm new_comm;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &procs_count);

    for ( i = procs_count - 1; i >= 0; i-- ) {
        ranks[ procs_count - 1 - i ] = i;
    }

    MPI_Comm_group(MPI_COMM_WORLD, &group);
    MPI_Group_incl(group, procs_count, ranks, &new_group);
    MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_rank(new_comm, &new_rank);

    printf ("World %3d New %3d\n", world_rank, new_rank); 
    
    MPI_Comm_free(&new_comm);
    MPI_Group_free(&new_group);
    MPI_Finalize(); 
   
    return 0; 
}