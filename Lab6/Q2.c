#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct thread_data {
    int start_row; // First row this thread should compute
    int end_row;   // Last row this thread should compute
    int N;         // Number of columns in matrixA and rows in matrixB
    int P;         // Number of columns in matrixB
    int *matrixA;  // Pointer to matrixA 
    int *matrixB;  // Pointer to matrixB 
    int *result;   // Pointer to the result matrix
};

void *multiply(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    for (int i = data->start_row; i <= data->end_row; i++) {
        for (int j = 0; j < data->P; j++) {
            int sum = 0;
            for (int k = 0; k < data->N; k++) {
                sum += data->matrixA[i * data->N + k] * data->matrixB[k * data->P + j];
            }
            data->result[i * data->P + j] = sum;
        }
    }
    pthread_exit(NULL);
}

int *read_matrix(const char *filename, int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }
    if (fscanf(file, "%d %d", rows, cols) != 2) {
        fprintf(stderr, "Invalid matrix dimensions in file %s\n", filename);
        fclose(file);
        return NULL;
    }
    int size = (*rows) * (*cols);
    int *matrix = malloc(size * sizeof(int));
    if (!matrix) {
        perror("malloc failed");
        fclose(file);
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        if (fscanf(file, "%d", &matrix[i]) != 1) {
            fprintf(stderr, "Failed to read element %d from %s\n", i, filename);
            free(matrix);
            fclose(file);
            return NULL;
        }
    }
    fclose(file);
    return matrix;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <matrix1_file> <matrix2_file> <num_threads>\n", argv[0]);
        return 1;
    }

    char *matrix1_file = argv[1];
    char *matrix2_file = argv[2];
    int num_threads = atoi(argv[3]);

    int M, N, N2, P;

    int *matrixA = read_matrix(matrix1_file, &M, &N);
    if (!matrixA)
        return 1;

    int *matrixB = read_matrix(matrix2_file, &N2, &P);
    if (!matrixB) {
        free(matrixA);
        return 1;
    }

    if (N != N2) {
        fprintf(stderr, "Matrices cannot be multiplied: %d vs %d\n", N, N2);
        free(matrixA);
        free(matrixB);
        return 1;
    }

    int *result = calloc(M * P, sizeof(int));
    if (!result) {
        perror("Failed to allocate result matrix");
        free(matrixA);
        free(matrixB);
        return 1;
    }

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    if (!threads) {
        perror("Failed to allocate threads");
        free(matrixA);
        free(matrixB);
        free(result);
        return 1;
    }

    struct thread_data **threads_data = malloc(num_threads * sizeof(struct thread_data *));
    if (!threads_data) {
        perror("Failed to allocate threads_data");
        free(threads);
        free(matrixA);
        free(matrixB);
        free(result);
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int rows_per_thread = M / num_threads;
    int remainder = M % num_threads;
    int start_row = 0;

    for (int i = 0; i < num_threads; i++) {
        threads_data[i] = malloc(sizeof(struct thread_data));
        if (!threads_data[i]) {
            perror("Failed to allocate thread data");
            for (int j = 0; j < i; j++) {
                free(threads_data[j]);
            }
            free(threads_data);
            free(threads);
            free(matrixA);
            free(matrixB);
            free(result);
            return 1;
        }
        threads_data[i]->start_row = start_row;
        threads_data[i]->end_row = start_row + rows_per_thread - 1;
        if (i < remainder) {
            threads_data[i]->end_row++;
        }
        threads_data[i]->N = N;
        threads_data[i]->P = P;
        threads_data[i]->matrixA = matrixA;
        threads_data[i]->matrixB = matrixB;
        threads_data[i]->result = result;

        if (pthread_create(&threads[i], NULL, multiply, threads_data[i]) != 0) {
            perror("Failed to create thread");
            for (int j = 0; j <= i; j++) {
                free(threads_data[j]);
            }
            free(threads_data);
            free(threads);
            free(matrixA);
            free(matrixB);
            free(result);
            return 1;
        }

        start_row = threads_data[i]->end_row + 1;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
    printf("Execution time: %.6f seconds\n", time_taken);

    FILE *out = fopen("multiplication.out", "w");
    if (!out) {
        perror("Failed to open output file");
        free(matrixA);
        free(matrixB);
        free(result);
        free(threads);
        for (int i = 0; i < num_threads; i++) {
            free(threads_data[i]);
        }
        free(threads_data);
        return 1;
    }
    fprintf(out, "%d %d\n", M, P);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            fprintf(out, "%d ", result[i * P + j]);
        }
        fprintf(out, "\n");
    }
    fclose(out);

    free(matrixA);
    free(matrixB);
    free(result);
    free(threads);
    for (int i = 0; i < num_threads; i++) {
        free(threads_data[i]);
    }
    free(threads_data);

    return 0;
}