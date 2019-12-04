#include <math.h>

__global__
void add(int n, float *x, float *y)
{
    for(int i = 0; i < n; i++)
    {
        y[i] = x[i] + y[i];
    }
}

int main(void)
{
    int N = 1 << 20;
    float *x, *y;
    cudaMallocManaged(&x, N*sizeof(float));
    cudaMallocManaged(&y, N*sizeof(float));
    for(int i = 0; i < N; i++)
    {
        x[i] = 1.0f;
        y[i] = 2.0f;
    }
    add<<<1, 1>>>(N, x, y);

    cudaDeviceSynchronize();
    float maxError = 0.0f;
    for (int i = 0; i < N; i++)
    {
        maxError = fmax(maxError, fabs(3.0f - y[i]));
    }
    printf("Max Error: ");
    printf("%f", maxError);

    cudaFree(x);
    cudaFree(y);
    
    return 0;   
}