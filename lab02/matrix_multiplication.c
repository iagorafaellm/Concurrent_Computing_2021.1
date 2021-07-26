/* Disciplina: Computação Concorrente - 2021.1 */
/* Prof.: Silvana Rossetto */
/* Estudante: Iago Rafael Lucas Martins */
/* Módulo 1 - Laboratório 2 */
/* Código: algoritmo concorrente para o problema de multiplicação de matrizes e que coleta informações sobre seu tempo de execução */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

//variáveis globais
int dim; //dimensão das matrizes de entrada
int nthreads; //número de threads a serem criadas
float *input_matrix1; //matriz de entrada 1
float *input_matrix2; //matriz de entrada 2
float *output_matrix; //matriz de saída

//alocação de memória para as estruturas de dados
void allocateMemory() {
    input_matrix1 = (float *) malloc(sizeof(float) * dim * dim);
    input_matrix2 = (float *) malloc(sizeof(float) * dim * dim);
    output_matrix = (float *) malloc(sizeof(float) * dim * dim);
    if (input_matrix1 == NULL || input_matrix2 == NULL || output_matrix == NULL) {
        printf("ERRO de malloc!\n");
        exit(2);
    }
}

//inicialização das estruturas de dados
void initMatrix() {
    for(int i = 0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            input_matrix1[i * dim + j] = 1; //equivalente a input_matrix1[i][j]
            input_matrix2[i * dim + j] = 4; //equivalente a input_matrix2[i][j]
        }
    }
}

//leitura e avaliação dos parâmetros de entrada + inicialização geral
void inicialization(int argc, char **argv) {
    if(argc < 3) {
        printf("Digite: %s <dimensão da matriz> <número de threads>\n", argv[0]);
        exit(1);
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);

    //corrige a possibilidade de threads desnecessárias
    if (nthreads > dim) {
        nthreads = dim;
    }
    allocateMemory();
    initMatrix();
}

//função executada pelas threads (eleva ao quadrado cada elemento de um array)
void * multiplyMatrices(void* arg) {
        long long linha = (long long) arg;

        for (long long i = linha; i < dim; i += nthreads)
                for (int j = 0; j < dim; j++)
                        for (int k = 0; k < dim; k++)
                                output_matrix[i * dim + j] += input_matrix1[i * dim + k] * input_matrix2[k * dim + j];

        pthread_exit(NULL);
}

//cria as threads
void createThread(pthread_t tid[]) {
    int args[nthreads];
    for(int i = 0; i < nthreads; i++) {
        args[i] = i;

        if (pthread_create(&tid[i], NULL, multiplyMatrices, (void *) args[i])) {
            printf("Erro na pthread_create()\n");
            exit(-1);
        }
    }
}

//espera as threads terminarem
void joinThread(pthread_t tid[]) {
    for(int i = 0; i < nthreads; i++) {        
        if (pthread_join(tid[i], NULL)) {
            printf("Erro na pthread_join()\n");
            exit(-2);
        }
    }
}

//verifica a corretude do código pelos valores da output_matrix
void verifyOutput() {
    float expectedOutput = 4 * dim;
    for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++)
                    if (output_matrix[i * dim + j] != expectedOutput) {
                            printf("Erro na linha %d coluna %d\nEsperava-se: %lf\nRecebeu-se: %lf\n", (i + 1), (j + 1), expectedOutput, output_matrix[i * dim + j]);
                            exit(-3);
                    }
}

//liberação da memória
void freeMemory() {
    free(input_matrix1);
    free(input_matrix2);
    free(output_matrix);
}

//fluxo principal
int main(int argc, char* argv[]) {
    //variável local à main
    double inicio, fim, delta, total_time_length = 0.0;

    GET_TIME(inicio);
    inicialization(argc, argv);
    pthread_t tid[nthreads]; //identificadores das threads
    GET_TIME(fim);
    delta = fim - inicio;
    total_time_length += delta;
    printf("Tempo de inicialização: %lf\n", delta);


    GET_TIME(inicio);
    createThread(tid);
    joinThread(tid);
    GET_TIME(fim);
    delta = fim - inicio;
    total_time_length += delta;
    printf("Tempo de multiplicação: %lf\n", delta);
    

    GET_TIME(inicio);
    verifyOutput();
    freeMemory();
    GET_TIME(fim);
    delta = fim - inicio;
    total_time_length += delta;
    printf("Tempo de finalização: %lf\n", delta);
    printf("Tempo total: %lf\n", total_time_length);

    return 0;
}