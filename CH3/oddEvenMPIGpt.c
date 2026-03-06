#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void merge_low(int *local, int *recv, int local_n)
{
    int *temp = (int*)malloc(local_n * sizeof(int));
    int i=0, j=0, k=0;

    while(k < local_n)
    {
        if(local[i] < recv[j])
            temp[k++] = local[i++];
        else
            temp[k++] = recv[j++];
    }

    for(i=0;i<local_n;i++)
        local[i] = temp[i];

    free(temp);
}

void merge_high(int *local, int *recv, int local_n)
{
    int *temp = (int*)malloc(local_n * sizeof(int));
    int i=local_n-1, j=local_n-1, k=local_n-1;

    while(k >= 0)
    {
        if(local[i] > recv[j])
            temp[k--] = local[i--];
        else
            temp[k--] = recv[j--];
    }

    for(i=0;i<local_n;i++)
        local[i] = temp[i];

    free(temp);
}

int main(int argc, char *argv[])
{
    int rank, size;
    int n = 8;

    int *arr = NULL;
    int *local_arr;
    int *recv_arr;

    int local_n;

    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    local_n = n/size;

    local_arr = (int*)malloc(local_n*sizeof(int));
    recv_arr = (int*)malloc(local_n*sizeof(int));

    if(rank == 0)
    {
        arr = (int*)malloc(n*sizeof(int));

        printf("Initial Array:\n");

        for(int i=0;i<n;i++)
        {
            arr[i] = rand()%100;
            printf("%d ",arr[i]);
        }

        printf("\n");
    }

    MPI_Scatter(arr, local_n, MPI_INT,
                local_arr, local_n, MPI_INT,
                0, MPI_COMM_WORLD);

    for(int i=0;i<local_n;i++)
        for(int j=i+1;j<local_n;j++)
            if(local_arr[i] > local_arr[j])
            {
                int t = local_arr[i];
                local_arr[i] = local_arr[j];
                local_arr[j] = t;
            }

    for(int phase=0; phase<size; phase++)
    {
        int partner;

        if(phase%2==0)
        {
            if(rank%2==0)
                partner = rank + 1;
            else
                partner = rank - 1;
        }
        else
        {
            if(rank%2==1)
                partner = rank + 1;
            else
                partner = rank - 1;
        }

        if(partner < 0 || partner >= size)
            continue;

        MPI_Sendrecv(local_arr, local_n, MPI_INT, partner, 0,
                     recv_arr, local_n, MPI_INT, partner, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if(rank < partner)
            merge_low(local_arr, recv_arr, local_n);
        else
            merge_high(local_arr, recv_arr, local_n);
    }

    MPI_Gather(local_arr, local_n, MPI_INT,
               arr, local_n, MPI_INT,
               0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        printf("\nSorted Array:\n");

        for(int i=0;i<n;i++)
            printf("%d ",arr[i]);

        printf("\n");
    }

    MPI_Finalize();

    return 0;
}