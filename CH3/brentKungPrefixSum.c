#include <stdio.h>

void brent_kung_scan(int A[], int n)
{
    int d;

    /* -------- UP-SWEEP -------- */
    for (d = 1; d < n; d = d * 2)
    {
        for (int i = 2*d - 1; i < n; i += 2*d)
        {
            A[i] = A[i] + A[i - d];
        }
    }

    /* -------- DOWN-SWEEP -------- */
    for (d = n/4; d >= 1; d = d / 2)
    {
        for (int i = 3*d - 1; i < n; i += 2*d)
        {
            A[i] = A[i] + A[i - d];
        }
    }
}

int main()
{
    int A[] = {3,1,7,0,4,1,6,3};
    int n = 8;

    brent_kung_scan(A,n);

    for(int i=0;i<n;i++)
        printf("%d ",A[i]);

    return 0;
}
