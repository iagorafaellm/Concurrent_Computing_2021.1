/* Disciplina: Computação Concorrente - 2021.1 */
/* Prof.: Silvana Rossetto */
/* Estudante: Iago Rafael Lucas Martins */
/* Módulo 3 - Laboratório 8 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

//variáveis globais
int messageCounter = 0;
pthread_mutex_t mutex;
sem_t welcomeCondition, comeBackCondition;

//thread 1: imprime a frase "Fique à vontade."
void *printMakeYorselfAtHome (void *t) {
    sem_wait(&welcomeCondition);
    printf("Fique à vontade.\n");
    sem_post(&comeBackCondition);

    pthread_exit(NULL);
}

//thread 2: imprime a frase "Boas vindas!"
void *printWelcome (void *t) {
    printf("Boas vindas!\n");
    sem_post(&welcomeCondition);
    sem_post(&welcomeCondition);

    pthread_exit(NULL);
}

//thread 3: imprime a frase "Volte sempre!"
void *printComeBack (void *t) {
    sem_wait(&comeBackCondition);
    sem_wait(&comeBackCondition);
    printf("Volte sempre!\n");

    pthread_exit(NULL);
}

//thread 4: imprime a frase "Sente-se, por favor."
void *printPleaseSit (void *t) {
    sem_wait(&welcomeCondition);
    printf("Sente-se, por favor.\n");
    sem_post(&comeBackCondition);

    pthread_exit(NULL);
}

void initialization () {
    sem_init(&welcomeCondition, 0, 0);
    sem_init(&comeBackCondition, 0, 0);
}

void taskStart () {
    pthread_t tids[NTHREADS];

    pthread_create(&tids[0], NULL, printWelcome, NULL);
    pthread_create(&tids[1], NULL, printMakeYorselfAtHome, NULL);
    pthread_create(&tids[2], NULL, printPleaseSit, NULL);
    pthread_create(&tids[3], NULL, printComeBack, NULL);

    for(int i = 0; i < NTHREADS; i++){
        pthread_join(tids[i], NULL);
    }
}

void freeDestroy () {
    free(tid);
    pthread_cond_destroy(&welcomeCondition);
    pthread_cond_destroy(&comeBackCondition);
}

int main () {
    initialization();
    taskStart();
    freeDestroy();

    return 0;
}