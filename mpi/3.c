#include "mpi.h"
#include <stdio.h>

int main(int argc, char* argv[]) 
{ 
    int rank, rank1, rank2, rank3, ranks1[3] = {0, 1, 2}, 
        ranks2[2] = {0, 1}, ranks3[2] = {2, 3};
    MPI_Group group, group1, group2, group3;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_group(MPI_COMM_WORLD, &group);
    MPI_Group_incl(group, 3, ranks1, &group1);
    MPI_Group_incl(group, 2, ranks2, &group2);
    MPI_Group_incl(group, 2, ranks3, &group3);

    MPI_Group_rank(group3, &rank3);
    MPI_Group_rank(group2, &rank2);
    MPI_Group_rank(group1, &rank1);

    if ((rank3 == MPI_UNDEFINED) && (rank2 != MPI_UNDEFINED) && (rank1 != MPI_UNDEFINED))
    {
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        printf("in 1 and 2 not 3: %d\n", rank);
    }

    MPI_Group_free(&group1);
    MPI_Group_free(&group2);
    MPI_Group_free(&group3);
    MPI_Finalize(); 
   
    return 0; 
}