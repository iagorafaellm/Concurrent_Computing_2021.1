/* Disciplina: Computação Concorrente - 2021.1 */
/* Prof.: Silvana Rossetto */
/* Estudante: Iago Rafael Lucas Martins */
/* Módulo 3 - Laboratório 9 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5 //tamanho do buffer

typedef long long int lli;

//variáveis globais
sem_t fullBuffer, emptySlot, mutex;
int producers; // Numero de threads produtoras
int consumers; // Numero de threads consumidoras
int buffer[BUFFER_SIZE];
int producers_index;

void init(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("Use da forma a seguir, poxa! :(\n%s <Nº de threads consumidoras (consumers)> <Nº de threads produtoras (producers)>\n", argv[0]);
		exit(1);
	}
	consumers = atoi(argv[1]);
	producers = atoi(argv[2]);

	producers_index = 0;

	sem_init(&fullBuffer, 0, 0);
	sem_init(&emptySlot, 0, BUFFER_SIZE);
	sem_init(&mutex, 0, 1);
}

void *producer(void *arg)
{
	lli id = (lli)arg;

	while (1)
	{
		printf("Produtor %lld na fila\n", id);

		sem_wait(&emptySlot);
		sem_wait(&mutex);

		printf("Produtor %lld produzindo\n", id);

		for (int i = 0; i < 100000000; i++)
			;
		for (int i = 0; i < 100000000; i++)
			;

		buffer[producers_index] = id + 1;
		producers_index++;

		if (producers_index == BUFFER_SIZE)
		{
			producers_index = 0;
			sem_post(&fullBuffer);
		}
		sem_post(&mutex);
	}
	pthread_exit(NULL);
}

void *consumer(void *arg)
{
	lli id = (lli)arg;

	while (1)
	{
		printf("Consumidor %lld na fila\n", id);

		sem_wait(&fullBuffer);
		sem_wait(&mutex);

		printf("Consumidor %lld consumindo\n", id);

		for (int i = 0; i < BUFFER_SIZE; i++)
		{
			printf("%d ", buffer[i]);
			buffer[i] = 0;
		}
		printf("\n");
		sem_post(&mutex);

		for (int i = 0; i < BUFFER_SIZE; i++)
		{
			sem_post(&emptySlot);
		}
	}
	pthread_exit(NULL);
}

pthread_t *createTid(int N)
{
	pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t) * N);
	if (tid == NULL)
	{
		puts("ERRO--createTid");
		exit(-1);
	}
	return tid;
}

void createThreads(pthread_t *tidCons, pthread_t *tidProd)
{
	for (lli i = 0; i < producers; i++)
	{
		if (pthread_create(tidProd + i, NULL, producer, (void *)i))
		{
			puts("ERRO--createThreads");
			exit(-1);
		}
	}

	for (lli i = 0; i < consumers; i++)
	{
		if (pthread_create(tidCons + i, NULL, consumer, (void *)i))
		{
			puts("ERRO--createThreads");
			exit(-1);
		}
	}
}

void waitThreads(pthread_t *tid, int N)
{
	for (int i = 0; i < N; i++)
	{
		if (pthread_join(tid[i], NULL))
		{
			puts("ERRO--pthread_join");
			exit(-1);
		}
	}
}

void destroy()
{
	sem_destroy(&fullBuffer);
	sem_destroy(&emptySlot);
	sem_destroy(&mutex);
}

int main(int argc, char *argv[])
{
	init(argc, argv);

	pthread_t *tidCons = createTid(consumers);
	pthread_t *tidProd = createTid(producers);
	createThreads(tidCons, tidProd);
	waitThreads(tidCons, consumers);
	waitThreads(tidProd, producers);

	destroy();
	return 0;
}