#include "mpi.h"
#include <stdio.h>

int main(int argc, char* argv[]) 
{ 
    int size, rank, *ranks, grank, i, source, dest, rc, count, tag = 1;
    MPI_Group group, group1, group2;
    MPI_Status Stat;
    char inmsg, outmsg = 'x';
    
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (i = 0; i < size; i += 2)
    {
        ranks[ i / 2 ] = i;
    }

    MPI_Comm_group(MPI_COMM_WORLD, &group);
    MPI_Group_incl(group, size / 2, ranks, &group1);
    MPI_Group_excl(group, size / 2, ranks, &group2);

    MPI_Group_rank(group1, &grank);
    if (grank != MPI_UNDEFINED)
    {
        MPI_Group_translate_ranks(group2, 1, &grank, group, &dest);

        MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
        MPI_Recv(&inmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &Stat);
    }
    else 
    {
        MPI_Group_rank(group2, &grank);

        MPI_Group_translate_ranks(group1, 1, &grank, group, &dest);

        MPI_Recv(&inmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &Stat);
        MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_count(&Stat, MPI_CHAR, &count);
    printf("Task %d: Received %d char(s) from task \
        %d with tag %d \n", rank, count, Stat.MPI_SOURCE, Stat.MPI_TAG);
    
    MPI_Group_free(&group1);
    MPI_Group_free(&group2);
    MPI_Finalize(); 
   
    return 0; 
}