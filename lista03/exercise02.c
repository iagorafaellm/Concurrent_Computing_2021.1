/* Disciplina: Computação Concorrente - 2021.1 */
/* Prof.: Silvana Rossetto */
/* Estudante: Iago Rafael Lucas Martins */
/* Terceira lista de exercícios - Exercício 02 */

//variáveis globais
sem_t s, d; //s inicializado com 1 e d inicializado com 0

//função consumidora
void *cons(void *args) {
    int item;
    sem_wait(&d);
    while(1) {
        sem_wait(&s);
        retira_item(&item);
        sem_post(&s);
        consome_item(item);
        sem_wait(&d);
    }
}

//função produtora
void *prod(void *args) {
    int item;
    while(1) {
        produz_item(&item);
        sem_wait(&s);
        insere_item(item);
        sem_post(&d);
        sem_post(&s);
    }
}