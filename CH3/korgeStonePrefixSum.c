#include <stdio.h>

void kogge_stone_scan(int A[], int n)
{
    int temp[n];

    for(int d = 1; d < n; d *= 2)
    {
        for(int i = 0; i < n; i++)
        {
            temp[i] = A[i];
        }

        for(int i = d; i < n; i++)
        {
            A[i] = temp[i] + temp[i-d];
        }
    }
}

int main()
{
    int A[] = {3,1,7,0,4,1,6,3};
    int n = 8;

    kogge_stone_scan(A,n);

    for(int i=0;i<n;i++)
        printf("%d ",A[i]);

    return 0;
}
