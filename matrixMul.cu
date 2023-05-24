#include <cstdlib>
#include <cassert>
#include <iostream>
#include <time.h>

using namespace std;


__global__ void matrixMul(int *a,int *b,int *c,int N){
    // Calculate the global row and column for each thread

    int row=blockIdx.y * blockDim.y + threadIdx.y;
    int col=blockIdx.x * blockDim.x + threadIdx.x;

    // Boundary check
    if(row<N && col<N){
        // Accumulate a partial result
        int temp=0;
        for(int i=0;i<N;i++){
            temp+=a[row*N + i] * b[i*N + col];
        }

        //Write back the result
        c[row*N + col]= temp;



    }

    

}

void init_matrix(int *m, int N){
    for(int i=0;i< N*N;i++){
        m[i]=rand()%100;
    }
}

void verify_result(int *a, int *b,int *c, int N){
    int tmp;

    for(int i=0;i<N;i++){
        
        for(int j=0;j<N;j++){
            tmp=0;
            for(int k=0;k<N;k++){
                tmp+=a[i*N + k] * b[k*N + j];
            }

            //check each result
            assert(tmp==c[i*N+j]);


        }
                             
    }

}

int main(){
     
    int N = 1<<10;
    size_t bytes = N*N* sizeof(int);

    

    int *a,*b,*c;
    cudaMallocManaged(&a,bytes);
    cudaMallocManaged(&b,bytes);
    cudaMallocManaged(&c,bytes);

    // Initialize our matrics
    init_matrix(a,N);
    init_matrix(b,N);

   

    int  threads=16;
    int blocks= (N + threads - 1) / threads;

   
    dim3 THREADS(threads,threads);
    dim3 BLOCKS(blocks,blocks);

    clock_t start, end;


    start = clock();
    matrixMul<<<BLOCKS, THREADS>>>(a,b,c,N);
    cudaDeviceSynchronize();
    end = clock();

    float GPUTime = ((float)(end-start))/CLOCKS_PER_SEC;
    cout<<"Parallel time "<<GPUTime<<endl;
    
    start = clock();
    //Verify the result
    verify_result(a,b,c,N);
    end = clock();

    float CPUTime=((float)(end-start))/CLOCKS_PER_SEC;

    cout<<"Sequential time "<<CPUTime<<endl;
    cout<<"Speed up "<<(CPUTime/GPUTime)<<endl;
    cout<<"Program Completed Successfully";
}
