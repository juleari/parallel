#include "mpi.h"
#include <stdio.h>

const int n = 4;

int main(int argc, char* argv[]) 
{ 
    int size, rank, i, j, coords[2], row, col, res;
    int arr1[n][n], arr2[n][n];
    int arr[n][n];
    int dims[2] = {n, n}, periods[2] = {0, 0};

    MPI_Comm comm_cart;
    MPI_Status stat;

    for ( i = 0; i < n; i++ )
        for ( j = 0; j < n; j++ )
        {
            arr1[i][j] = 1;
            arr2[i][j] = 2;
        }
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm_cart);

    MPI_Cart_coords( comm_cart, rank, 2, coords );

    row = coords[0];
    col = coords[1];

    res = 0;
    for ( i = 0; i < n; i++ )
    {
        res += arr1[row][i] * arr2[i][col];
    }

    if ( rank != 0 ) 
    {
        MPI_Send( &res, 1, MPI_INT, 0, rank, comm_cart );
    }
    else {

        arr[row][col] = res;

        for (i = 1; i < size; i++)
        {
            MPI_Cart_coords( comm_cart, i, 2, coords );
            MPI_Recv( &arr[coords[0]][coords[1]], 1, MPI_INT, i, i, comm_cart, &stat );
        }
        
        for ( i = 0; i < n; i++ )
        {   
            for ( j = 0; j < n; j++ )
                printf("%d ", arr[i][j]);
            printf("\n");
        }
    }

    MPI_Finalize(); 
   
    return 0; 
}