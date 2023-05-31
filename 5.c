/*
nome: Felipe Barbosa 
      Victoria Caroline 
05) Implemente um programa paralelo que multiplica duas matrizes 1000x1000.

*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define n 1000 // tamanho das matrizes

double *M, *M1, *R; // matrizes
int num_threads; // numero de threads a serem usadas

void* multiply_matrices(void* arg) {
    int thread_id = *(int*) arg;
    int chunk_size = n / num_threads; // tamanho do bloco para cada thread
    int start = thread_id * chunk_size; // inicio do bloco para a thread
    int end = start + chunk_size; // fim do bloco para a thread

    int i, j, k;
    for (k = start; k < end; k++) { // percorre as linhas da matriz resultado que a thread deve calcular
        for (i = 0; i < n; i++) { // percorre as linhas da matriz M1
            for (j = 0; j < n; j++) { // percorre as colunas da matriz M e as linhas da matriz M1
                R[k * n + i] += M[k * n + j] * M1[j * n + i]; // calcula o elemento da matriz resultado
            }
        }
    }

    pthread_exit(NULL);
}
int main(void) {
    M = (double*) malloc(sizeof(double) * n * n);
    M1 = (double*) malloc(sizeof(double) * n * n);
    R = (double*) malloc(sizeof(double) * n * n);
    memset(M, 0, sizeof(double) * n * n);
    memset(M1, 0, sizeof(double) * n * n);
    memset(R, 0, sizeof(double) * n * n);

    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            M[i * n + j] = (0.1 + (j + i)) / 10;
            M1[i * n + j] = (0.1 + (0.20 * j + 0.10 * i)) / 10;
        }
    }

   // Define o numero de threads
    num_threads = 1;
    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    // Começa a medir o tempo de execução
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    // Cria as threads
    for (i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        int ret = pthread_create(&threads[i], NULL, multiply_matrices, &thread_ids[i]);
        if (ret) {
            fprintf(stderr, "Error: unable to create thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Aguarda as threads terminarem
    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Para de medir o tempo de execução
    clock_gettime(CLOCK_REALTIME, &end);
    double secs = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

	// Imprime a matriz resultado
    printf("\n\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%f\t", R[i * n + j]);
        }
        printf("\n");
    }
    printf("\n\n%.16g seconds produto A*B\n", secs);
    return 0;
}

