/* Disciplina: Computção Concorrente - 2021.1 */
/* Prof.: Silvana Rossetto */
/* Estudante: Iago Rafael Lucas Martins */
/* Módulo 1 - Laboratório 1 */
/* Código: elevar ao quadrado cada elemento de um array de 10000 elementos usando duas threads (além da thread principal) */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS 2 //total de threads a serem criadas
#define ARRAYSIZE 10000 //quantidade de elementos do vetor

//variáveis globais
int array[ARRAYSIZE];
pthread_t tid[NTHREADS]; //identificadores das threads

//inicializa o array com valores sequenciais de 1 a 10000
void initArray() {
    for(int i = 0; i < ARRAYSIZE; i++) {
        array[i] = i+1;
    }
}

//função executada pelas threads (eleva ao quadrado cada elemento de um array)
void *squareEllements (void *arg) {
    int first_element = *((int *) arg);

    //printf("Primeiro elemento = %d.\n", first_element);

    for(int i = first_element; i < ARRAYSIZE; i+=2) {
        array[i] = array[i] * array[i];
    }

    free(arg); //aqui pode liberar a alocação feita na main
    pthread_exit(NULL);
}

//cria as threads
void createThread() {
    for(int i = 0; i < NTHREADS; i++) {
        int *thread = malloc(sizeof(int));
        *thread = i;

        if (pthread_create(&tid[i], NULL, squareEllements, (void *) thread)) {
            printf("Erro na pthread_create()\n"); exit(-1);
        }
    }
}

//espera as threads terminarem
void joinThread() {
    for(int i = 0; i < NTHREADS; i++) {        
        if (pthread_join(tid[i], NULL)) {
            printf("Erro na pthread_join()\n");
            exit(-1);
        }
    }
}

//verifica se os valores finais do array estão corretos
void verifyArray() {
    for(int i = 0; i < ARRAYSIZE; i++) {
        int element_expected = (i+1) * (i+1);
        if(array[i] != element_expected) {
            printf("Erro na posição %d. Esperávamos %d, recebemos %d.\n", i, element_expected, array[i]);
            return;
        }
    }
    printf("A verificação não achou erros.\n");
}

//chama todas as funções
void callFunctions() {

    initArray();

    createThread();

    joinThread();

    verifyArray();
}

//função principal do programa
int main() {
    
    callFunctions();
    
    return 0;
}
