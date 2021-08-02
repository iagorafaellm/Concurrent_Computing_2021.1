/* Disciplina: Computação Concorrente - 2021.1 */
/* Prof.: Silvana Rossetto */
/* Estudante: Iago Rafael Lucas Martins */
/* Módulo 1 - Laboratório 3 */
/* Código: uma solução sequencial e uma solução concorrente para o problema de encontrar o menor e o maior valor em um vetor de números reais */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <float.h>
#include <time.h>
#include "../lib/timer.h"

typedef long long int lli;

typedef struct {
	float minimum, maximum;
} _limits;

//variáveis globais
float* array;
lli size;
int nthreads;

float minimum(float a, float b) {
	return a < b? a : b;
}

float maximum(float a, float b) {
	return a > b? a : b;
}

float randomFloat() {	
	return ((float) rand()) / ((float)RAND_MAX) * 1000.0;
}

//alocação de memória para o array
void allocateMemory() {
	array = (float *) malloc(sizeof(float) * size);
	if (array == NULL) {
		fprintf(stderr, "ERRO--malloc");
		exit(2);
	}
}

//inicialização do array
void initArray() {
	for (lli i = 0ll; i < size; i++) 
		array[i] = randomFloat();
}

//leitura e avaliação dos parâmetros de entrada + inicialização geral
void inicialization(int argc, char* argv[]) {
	if (argc < 3) {
		printf("Digite: %s <tamanho do vetor> <número de threads>\n", argv[0]);
		exit(1);
	}
	size = atoll(argv[1]);
	nthreads = atoi(argv[2]);
	nthreads = (nthreads <= size)? nthreads : size;

	allocateMemory();
	srand(time(NULL));
	initArray();
}

_limits* initLimits() {
	_limits* limits = (_limits *) malloc(sizeof(_limits));
     	if (limits == NULL) {
		fprintf(stderr, "ERRO--malloc limits\n");
		exit(3);
	}
	limits -> minimum = FLT_MAX;
	limits -> maximum = FLT_MIN;
	return limits;	

}

_limits* findLimitsSeq() {
	_limits* limits = initLimits();
	
	for (lli i = 0ll; i < size; i++) {
		limits -> minimum = minimum(limits -> minimum, array[i]);
		limits -> maximum = maximum(limits -> maximum, array[i]);
	}
	return limits;	
}

void* findConc(void* args) {
	lli inicio = (lli) args;
	_limits* limits = initLimits();

	for (lli i = inicio; i < size; i+=(lli)nthreads) {
		limits -> minimum = minimum(limits -> minimum, array[i]);
		limits -> maximum = maximum(limits -> maximum, array[i]);
	}
	
	pthread_exit((void *) limits);

}

//cria as ids das Threads
pthread_t* createTid() {
	pthread_t* tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if (tid == NULL) {
		fprintf(stderr, "ERRO--malloc\n");
		exit(4);
	}
	return tid;
}

//cria as threads
void createThread(pthread_t* tid) {
	for (lli i = 0ll; i < nthreads; i++) {
		if (pthread_create(tid+i, NULL, findConc, (void *) i)) {
			fprintf(stderr, "ERRO--pthread_create\n");
			exit(5);
		}
	}
	
}

//espera as threads terminarem
_limits* joinThread(pthread_t* tid) {
	_limits* limits = initLimits();
	_limits* threadLimits;
	for (lli i = 0; i < nthreads; i++) {
		if (pthread_join(*(tid+i), (void **) &threadLimits)) {
			fprintf(stderr, "ERRO--pthread_join\n");
			exit(6);
		}	
		limits -> minimum = minimum(limits -> minimum, threadLimits -> minimum);
		limits -> maximum = maximum(limits -> maximum, threadLimits -> maximum);
		free(threadLimits);
	}
	free(tid);

	return limits;
}

//função para organizar as três funções anteriores
_limits* findLimitsConc() {
	pthread_t* tid = createTid();
	createThread(tid);
	return joinThread(tid);
}

int main(int argc, char* argv[]) {
	//variáveis locais à main
	double inicio, fim, totalSequencial, totalConcurrent;

	_limits* limitsSeq,* limitsConc;

	GET_TIME(inicio);
    inicialization(argc, argv);
	limitsSeq = findLimitsSeq();
	GET_TIME(fim);
	totalSequencial = fim - inicio;

	GET_TIME(inicio);
	limitsConc = findLimitsConc();
	free(array);
	GET_TIME(fim);
	totalConcurrent = fim - inicio;

	printf("SEQUENCIAL\nMáximo: %f\nMínimo: %f\n\n", limitsSeq -> maximum, limitsSeq -> minimum); 
	printf("CONCORRENTE\nMáximo: %f\nMínimo: %f\n\n", limitsConc -> maximum, limitsConc -> minimum);
	printf("TOTAL SEQUENCIAL: %lf\n", totalSequencial);
	printf("TOTAL CONCORRENTE: %lf\n", totalConcurrent);
	printf("RAZÃO DE ACELERAÇÃO: %lf\n", totalSequencial/totalConcurrent);

	return 0;
}
