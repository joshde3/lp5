#include<bits/stdc++.h>
#include<omp.h>
using namespace std;
using namespace std::chrono;

void printArray(int arr[], int n){
    for (int i = 0; i < n; i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;   
}

void bubbleSort(int arr[],int n){
    for (int i = 0; i < n-1; i++)
    {
        for (int j = 0; j < n-i-1; j++)
        {
            if(arr[j]>arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
        
    }
    
}

void swap_para(int *a, int *b){
    int temp=*a;
    *a=*b;
    *b=temp;
}

// void bubbleSort_para(int arr[], int n){
//     int i,j,first;
//     for (int i = 0; i < n-1; i++)
//     {
//         first=i%2;
//         #pragma omp parallel for default(none), shared(arr,first,n)
//         for (int j = first; j < n-1; j++)
//         {
//             if(arr[j]>arr[j+1]){
//                 swap_para(&arr[j],&arr[j+1]);
//             }
            
//         }
        
//     }
    
// }



void bubbleSort_para(int arr[], int n)
{
    bool sorted = false;
    int i;

    while (!sorted)
    {
        sorted = true;

        #pragma omp parallel for default(none) shared(arr, n, sorted) private(i)
        for (i = 0; i < n - 1; i += 2)
        {
            if (arr[i] > arr[i + 1])
            {
                std::swap(arr[i], arr[i + 1]);
                #pragma omp critical
                sorted = false;
            }
        }

        #pragma omp parallel for default(none) shared(arr, n, sorted) private(i)
        for (i = 1; i < n - 1; i += 2)
        {
            if (arr[i] > arr[i + 1])
            {
                std::swap(arr[i], arr[i + 1]);
                #pragma omp critical
                sorted = false;
            }
        }
    }
}


// void merge(int arr[], int p, int q, int r){
//     int size1=q-p+1;
//     int size2=r-q;
//     int left[size1], right[size2];
//     for(int i=0;i<size1;i++){
//         left[i]=arr[p+i];
//     }
//     for(int j=0;j<size2;j++){
//         right[j]=arr[q+j+1];
//     }
//     int i=0,j=0,k=p;

//     while(i<size1 and j<size2){
//         if(left[i]<right[j]){
//             arr[k]=left[i];
//             i++;
//         }
//         else{
//             arr[k]=right[j];
//             j++;
//         }
//         k++;
//     }

//     while(i<size1){
//         arr[k]=arr[i];
//         i++;
//         k++;
//     }

//     while(j<size2){
//         arr[k]=arr[j];
//         j++;
//         k++;
//     }

// }

void merge(int arr[], int p, int q, int r) {
    int n1 = q - p + 1;
    int n2 = r - q;

    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[p + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[q + 1 + j];

    int i = 0, j = 0, k = p;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}


void mergesort(int arr[], int l, int r){
    if(l<r){
        int m = l+(r-l)/2;
        mergesort(arr,l,m);
        mergesort(arr,m+1,r);
        merge(arr,l,m,r);
    }
}


void mergesort_para(int arr[], int l, int r){
    if(l<r){
        int m = l+(r-l)/2;
        #pragma omp parallel sections num_threads(2)
        {
            #pragma omp section
            {
                mergesort(arr,l,m);
            }
            #pragma omp section
            {
                mergesort(arr,m+1,r);
            }
        }
        merge(arr,l,m,r);
    }
}




int main(){
    int n;
    cout<<"Enter number of elements : ";
    cin>>n;
    int arr[n],arr1[n],arr2[n],arr3[n],arr4[n];
    for (int i=0;i<n;i++){
        arr[i]=rand()%100;
        arr1[i]=arr[i];
        arr2[i]=arr[i];
        arr3[i]=arr[i];
        arr4[i]=arr[i];
    }

    printArray(arr,n);

    
    auto start_time_bubble_sort_sequential = high_resolution_clock::now();
    bubbleSort(arr1,n);
    auto end_time_bubble_sort_sequential = high_resolution_clock::now();
    auto time_bubble_sort_sequential = duration_cast<microseconds>(end_time_bubble_sort_sequential - start_time_bubble_sort_sequential).count();
    printArray(arr1,n);
    
    auto start_time_bubble_sort_para = high_resolution_clock::now();
    bubbleSort_para(arr2,n);
    auto end_time_bubble_sort_para = high_resolution_clock::now();
    auto time_bubble_sort_para = duration_cast<microseconds>(end_time_bubble_sort_para - start_time_bubble_sort_para).count();
    printArray(arr2,n);

    auto start_time_merge_sort_seq = high_resolution_clock::now();
    mergesort(arr3,0,n);
    auto end_time_merge_sort_seq = high_resolution_clock::now();
    auto time_merge_sort_seq = duration_cast<microseconds>(end_time_merge_sort_seq - start_time_merge_sort_seq).count();
    printArray(arr3,n);

    auto start_time_merge_sort_para = high_resolution_clock::now();
    mergesort_para(arr4,0,n);
    auto end_time_merge_sort_para = high_resolution_clock::now();
    auto time_merge_sort_para = duration_cast<microseconds>(end_time_merge_sort_para - start_time_merge_sort_para).count();
    printArray(arr4,n);

    cout<<"Bubble sort seq : "<<time_bubble_sort_sequential<<" ms\n";
    cout<<"Bubble sort para : "<<time_bubble_sort_para<<" ms\n";
    cout<<"Merge sort seq : "<<time_merge_sort_seq<<" ms\n";
    cout<<"merge sort para : "<<time_merge_sort_para<<" ms\n";

    cout<<"bubble sort speedup : "<<(double)time_bubble_sort_sequential/(double)time_bubble_sort_para<<"\n";
    cout<<"merge sort speedup : "<<(double)time_merge_sort_seq/(double)time_merge_sort_para<<"\n";

    return 0; 
}