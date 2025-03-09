#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

#define Array_Size 100000

//Merge Function
void Merge(int arr[], int left, int mid, int right){

    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *Left = (int*)malloc(n1 * sizeof(int));
    int *Right = (int*)malloc(n2 * sizeof(int));

    if (Left == NULL || Right == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    for (int i = 0; i < n1; i++){
        Left[i] = arr[left + i];
    }

    for (int i = 0; i < n2; i++){
        Right[i] = arr[mid + 1 + i];
    }

    int i=0, j=0, k=left;

    while (i < n1 && j < n2){
        if ( Left[i] <= Right[j]){
            arr[k] = Left[i];
            i++;
        }
        else{
            arr[k] = Right[j];
            j++;
        }
        k++;
    }

    while (i < n1){
        arr[k] = Left[i];
        i++;
        k++;
    }

    while (j < n2){
        arr[k] = Right[j];
        j++;
        k++;
    }
    
    free(Left);
    free(Right);

}

//Merge sort function using openMp
void MergeSort(int arr[], int left, int right, int is_static){

    if (left < right){

        int mid = left + (right - left) / 2;

        //Threshold to prevent too many tasks
        if (right - left < 1000){
            MergeSort(arr, left, mid, is_static);
            MergeSort(arr, mid + 1, right, is_static);
        }
        else{

            #pragma omp task shared(arr) if(is_static == 0)
            {
                MergeSort(arr, left, mid, is_static);
            }
            
            #pragma omp task shared(arr) if(is_static == 0)
            {
                MergeSort(arr, mid + 1, right, is_static);
            }

            #pragma omp taskwait // this line make sure that both tasks are done before merging

        }

        Merge(arr, left, mid, right);

    }

}

int main(){

    int arr[Array_Size];
    int arr_copy[Array_Size];  //Copy of the original array for dynamic scheduling
    srand(time(0));

    printf("Generating 100,000 random numbers ...\n");

    for (int i = 0; i < Array_Size; i++){

        arr[i] = rand() % 90000 + 10000; //Generate random 5-digit numbers in the range [10000, 99999]
        arr_copy[i] = arr[i]; //copy of original array for dynamic scheduling

    }

    // Static Scheduling

    printf("Sorting array using merge sort with Static scheduling...\n");

    double start_time_static = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            MergeSort(arr, 0, Array_Size - 1, 1); // 1 for static scheduling
        }
    }

    double end_time_static = omp_get_wtime();

    double ExecutionTime_static = end_time_static - start_time_static;

    printf("----- Static Execution Time -----\n");
    printf("Execution Time : %.6f seconds\n", ExecutionTime_static);

    // Dynamic Scheduling

    printf("Sorting array using merge sort with Dynamic scheduling...\n");

    double start_time_dynamic = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            MergeSort(arr, 0, Array_Size - 1, 0); // 0 for dynamic scheduling
        }
    }

    double end_time_dynamic = omp_get_wtime();

    double ExecutionTime_dynamic = end_time_dynamic - start_time_dynamic;

    printf("----- Dynamic Execution Time -----\n");
    printf("Execution Time : %.6f seconds\n", ExecutionTime_dynamic);

    return 0;

}