#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

struct message
{
    int num;
    char name[MPI_MAX_PROCESSOR_NAME];
};

int main(int argc, char* argv[]) 
{ 
    int rank, size, len, tag = 1, rc, i, 
        lens[2] = { 1, MPI_MAX_PROCESSOR_NAME};
    
    struct message mess, *buff;
    
    MPI_Datatype type, types[2] = { MPI_INT, MPI_CHAR };
    MPI_Aint offsets[2] = {0, 1};
    
    MPI_Init(&argc, &argv);
    MPI_Type_create_struct(2, lens, offsets, types, &type);
    MPI_Type_commit(&type);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    mess.num = rank;
    MPI_Get_processor_name( mess.name, &len );

    if ( rank == 0 )
    {   
        buff = malloc(size * (sizeof(int) + lens[1]));
    }

    MPI_Gather( &mess, 1, type, buff, 1, type, 0, MPI_COMM_WORLD );

    if ( rank == 0 )
        for ( i = 0; i < size; i++ )
            printf("num: %d, name: %s\n", buff[i].num, buff[i].name);
    
    MPI_Finalize(); 
   
    return 0; 
}