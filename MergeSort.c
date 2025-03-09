#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define Array_Size 100000

// Merge Function
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

//Merge Sort Function
void MergeSort(int arr[], int left, int right){

    if (left < right){
        
        int mid = left + (right - left) / 2;
        MergeSort(arr, left, mid);
        MergeSort(arr, mid + 1, right);
        Merge(arr, left, mid, right);

    }

}

int main(){

    int arr[Array_Size];
    srand(time(0)); // seed for the generation of random numbers

    printf("Generating 100,000 random numbers ...\n");

    for (int i = 0; i < Array_Size; i++){

        arr[i] = rand() % 90000 + 10000; //Generate random 5-digit numbers in the range [10000, 99999]

    }

    printf("Sorting array using merge sort ...\n");

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start); // High resolution Timer
    MergeSort(arr, 0, Array_Size - 1);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double ExecutionTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("----- Execution Time -----\n");
    printf("Execution Time : %.6f seconds\n", ExecutionTime);

    return 0;

}