#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Global matrices - using static allocation with MAX = 20
#define MAX 20  // Hardcoded as specified in requirements
#define NUM_THREADS 10

int matA[MAX][MAX];
int matB[MAX][MAX];
int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX];
int matProductResult[MAX][MAX];

// Structure to pass thread arguments
typedef struct {
    int thread_id;
    int start_row;
    int end_row;
} ThreadData;

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Thread function for matrix addition
void* computeSum(void* args) {
    ThreadData *data = (ThreadData*)args;
    
    // This thread processes rows from start_row to end_row-1
    for(int i = data->start_row; i < data->end_row; i++) {
        for(int j = 0; j < MAX; j++) {
            matSumResult[i][j] = matA[i][j] + matB[i][j];
        }
    }
    
    free(data);
    pthread_exit(NULL);
}

// Thread function for matrix subtraction
void* computeDiff(void* args) {
    ThreadData *data = (ThreadData*)args;
    
    for(int i = data->start_row; i < data->end_row; i++) {
        for(int j = 0; j < MAX; j++) {
            matDiffResult[i][j] = matA[i][j] - matB[i][j];
        }
    }
    
    free(data);
    pthread_exit(NULL);
}

// Thread function for matrix multiplication
void* computeProduct(void* args) {
    ThreadData *data = (ThreadData*)args;
    
    for(int i = data->start_row; i < data->end_row; i++) {
        for(int j = 0; j < MAX; j++) {
            matProductResult[i][j] = 0;
            for(int k = 0; k < MAX; k++) {
                matProductResult[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    
    free(data);
    pthread_exit(NULL);
}

// Helper function to create threads for an operation
void runMatrixOperation(void* (*thread_func)(void*), const char* operation_name) {
    pthread_t threads[NUM_THREADS];
    ThreadData *thread_data[NUM_THREADS];
    
    // Calculate rows per thread (20 rows / 10 threads = 2 rows per thread)
    int rows_per_thread = MAX / NUM_THREADS;
    
    printf("Computing %s with %d threads...\n", operation_name, NUM_THREADS);
    
    // Create threads
    for(int i = 0; i < NUM_THREADS; i++) {
        thread_data[i] = (ThreadData*)malloc(sizeof(ThreadData));
        thread_data[i]->thread_id = i;
        thread_data[i]->start_row = i * rows_per_thread;
        thread_data[i]->end_row = (i == NUM_THREADS - 1) ? MAX : (i + 1) * rows_per_thread;
        
        pthread_create(&threads[i], NULL, thread_func, (void*)thread_data[i]);
    }
    
    // Wait for all threads to finish
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    srand(time(0));  // Seed random number generator
    
    printf("Parallel Matrix Operations\n");
    printf("Matrix Size: %dx%d\n", MAX, MAX);
    printf("Number of Threads: %d\n\n", NUM_THREADS);
    
    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);
    
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads (done in runMatrixOperation)
    
    // 4. Create threads and compute results
    runMatrixOperation(computeSum, "Sum");
    runMatrixOperation(computeDiff, "Difference");
    runMatrixOperation(computeProduct, "Product");
    
    // 5. Wait for all threads to finish (done in runMatrixOperation)
    
    // 6. Print the results.
    printf("\nResults:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    
    return 0;
}