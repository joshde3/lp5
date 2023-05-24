#include <iostream>
#include <cstdlib>
#include <time.h>


using namespace std;




void init(int *a,int n){
	
	for(int i=0;i<n;i++){
		a[i]=i;
	}

}

bool verify(int *a,int *b,int *c,int n){

	int s=0;
	for(int i=0;i<n;i++){
		s=a[i]+b[i];
		if(s!=c[i])
		return false;
	}
	return true;
	
}

__global__ void vectorAdd(int *a,int *b,int *c,int n){

	int i=blockIdx.x * blockDim.x + threadIdx.x;
	int temp=0;
	if(i<n){
		temp+=a[i]+b[i];
	}
	c[i]=temp;

}

int main(){

	int N=1<<10;
	int *a,*b,*c;
	
	cudaMallocManaged(&a,N*sizeof(int));
	cudaMallocManaged(&b,N*sizeof(int));
	cudaMallocManaged(&c,N*sizeof(int));
	init(a,N);
	init(b,N);
	
	int thread=16;
	int blockx=(N+thread-1)/thread;
	
	clock_t start,end;
	start=clock();
	vectorAdd<<<blockx,thread>>>(a,b,c,N);
	end=clock();
	
	cudaDeviceSynchronize();
	double tp=end-start;
	
	start=clock();
	int res=verify(a,b,c,N);
	end=clock();
	
	double ts=end-start;
	
	if(verify(a,b,c,N)){
		cout<<"Verified "<<endl;
		cout<<"Speedup "<<ts/tp<<endl;
	}
	else{
		cout<<"UnSucessful";
	}
	
	return 0;


}
