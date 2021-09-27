/* Disciplina: Computação Concorrente - 2021.1 */
/* Prof.: Silvana Rossetto */
/* Estudante: Iago Rafael Lucas Martins */
/* Segunda lista de exercícios - Exercício 02 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//variáveis globais
long long int contador = 0;
pthread_mutex_t mutex;
pthread_cond_t multCondition; //condição do valor de contador ser múltiplo de 100

void FazAlgo (int c) { }

void *T1 (void * arg) {
    while (1) {
        FazAlgo(contador);
        pthread_mutex_lock(&mutex); //tranca e possibilita o uso da variável global contador
        contador++;

        if (contador % 100 == 0) { //sendo múltiplo, ele destranca a condição e fica na espera
            pthread_cond_signal(&multCondition);
            pthread_cond_wait(&multCondition, &mutex);
        }
        pthread_mutex_unlock(&mutex);
    }
}

void *T2 (void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex); //assim como na na T1, usei o lock para trancar e possibilitar o uso da variável global contador

        if (contador % 100 == 0) { // Se for multiplo imprime e libera a condicao pra T1 poder somar
            printf("%lld\n", contador);
            pthread_cond_signal(&multCondition);
        }
        pthread_cond_wait(&multCondition, &mutex); // Espera a condicao estar liberada novamente
        pthread_mutex_unlock(&mutex);
    }
}

int main () {
    //variável local
    pthread_t tid1, tid2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&multCondition, NULL);

    if (pthread_create(&tid1, NULL, T1, NULL) && pthread_create(&tid2, NULL, T2, NULL)) {
        printf("ERROR... pthread_create");
        return 1;
    }

    if (pthread_join(tid1, NULL) && pthread_join(tid2, NULL)) {
        printf("ERROR... pthread_join");
        return 2;
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&multCondition);

    return 0;
}