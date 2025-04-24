#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *A, *B, *C;
int M, N, P; // Matrix A: MxN, B: NxP
int num_threads;

void *multiply(void *arg) {
    int thread_id = *(int *)arg;
    free(arg);
    int rows_per_thread = M / num_threads;
    int start = thread_id * rows_per_thread;
    int end = (thread_id == num_threads - 1) ? M : start + rows_per_thread;

    for (int i = start; i < end; ++i) {
        for (int j = 0; j < P; ++j) {
            C[i * P + j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i * P + j] += A[i * N + k] * B[k * P + j];
            }
        }
    }

    return NULL;
}

int *read_matrix(const char *file, int *rows, int *cols) {
    FILE *fp = fopen(file, "r");
    fscanf(fp, "%d %d", rows, cols);
    int *mat = malloc((*rows) * (*cols) * sizeof(int));
    for (int i = 0; i < (*rows) * (*cols); ++i)
        fscanf(fp, "%d", &mat[i]);
    fclose(fp);
    return mat;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s matrix1.txt matrix2.txt num_threads\n", argv[0]);
        return 1;
    }

    A = read_matrix(argv[1], &M, &N);
    B = read_matrix(argv[2], &N, &P);
    C = calloc(M * P, sizeof(int));
    num_threads = atoi(argv[3]);

    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; ++i) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&threads[i], NULL, multiply, arg);
    }

    for (int i = 0; i < num_threads; ++i)
        pthread_join(threads[i], NULL);

    FILE *out = fopen("result.txt", "w");

    fprintf(out, "%d %d\n", M, P);
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < P; ++j)
            fprintf(out, "%d ", C[i * P + j]);
        fprintf(out, "\n");
    }

    fclose(out);
    printf("Result written to result.txt\n");
    return 0;
}
