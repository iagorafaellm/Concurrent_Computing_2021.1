/* Disciplina: Computação Concorrente - 2021.1 */
/* Prof.: Silvana Rossetto */
/* Estudante: Iago Rafael Lucas Martins */
/* Módulo 2 - Laboratório 4 */
/* Código: projetar e implementar um programa concorrente onde a ordem de execução das threads é controlada no programa. */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4

//variáveis globais
int messageCounter = 0;
pthread_mutex_t mutex;
pthread_cond_t welcomeCondition, comeBackCondition;

//thread 1: imprime a frase "Fique à vontade."
void *printMakeYorselfAtHome (void *t) {
    pthread_mutex_lock(&mutex);

    if(messageCounter == 0) {
        pthread_cond_wait(&welcomeCondition, &mutex);
    }

    printf("Fique à vontade.\n");
    messageCounter++;
    pthread_cond_signal(&comeBackCondition);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

//thread 2: imprime a frase "Boas vindas!"
void *printWelcome (void *t) {
    printf("Boas vindas!\n");

    pthread_mutex_lock(&mutex);
    messageCounter++;
    pthread_cond_broadcast(&welcomeCondition);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

//thread 3: imprime a frase "Volte sempre!"
void *printComeBack (void *t) {
    pthread_mutex_lock(&mutex);

    while(messageCounter != 3) {
        pthread_cond_wait(&comeBackCondition, &mutex);
    }

    printf("Volte sempre!\n");
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

//thread 4: imprime a frase "Sente-se, por favor."
void *printPleaseSit (void *t) {
    pthread_mutex_lock(&mutex);

    if(messageCounter == 0) {
        pthread_cond_wait(&welcomeCondition, &mutex);
    }

    printf("Sente-se, por favor.\n");
    messageCounter++;
    pthread_cond_signal(&comeBackCondition);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

//cria as ids das Threads
pthread_t* createTid () {
	pthread_t* tid = (pthread_t *) malloc(sizeof(pthread_t) * NTHREADS);
    
	if (tid == NULL) {
		printf("ERRO--malloc\n");
		exit(1);
	}

	return tid;
}

pthread_t* initialization () {
  pthread_t *tid;
  tid = createTid();

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&welcomeCondition, NULL);
  pthread_cond_init(&comeBackCondition, NULL);

  return tid;
}

void taskStart (pthread_t *tid){
  pthread_create(tid, NULL, printMakeYorselfAtHome, NULL);
  pthread_create(tid+1, NULL, printWelcome, NULL);
  pthread_create(tid+2, NULL, printComeBack, NULL);
  pthread_create(tid+3, NULL, printPleaseSit, NULL);

  for(int i = 0; i < NTHREADS; i++){
    pthread_join(tid[i], NULL);
  }
}

int main (){
  pthread_t *tid;

  tid = initialization();
  taskStart(tid);

  free(tid);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&welcomeCondition);
  pthread_cond_destroy(&comeBackCondition);

  return 0;
}