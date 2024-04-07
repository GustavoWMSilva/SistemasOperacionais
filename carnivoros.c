#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct {
    int M;
    int N;
    int indice;
} ParametrosCanibal;

bool *interesse;
int vez = 0;

int pratos_sujos = 0;
sem_t mutex, cheia, cozinhando;

void *canibais(void *arg)
{
    ParametrosCanibal *params = (ParametrosCanibal *)arg;
    int M = params->M;
    int N = params -> N;
    long int id = params->indice;

    while (1) {
        interesse[id] = true;
        vez = (id + 1) % N;
        while (interesse[vez] && vez != id)
            ;
        

        sem_wait(&cozinhando);
        sem_wait(&mutex);
        printf("\nCanibal %ld comeu. Pratos sujos: %d", id, pratos_sujos);
        pratos_sujos++;
        if (pratos_sujos == M)
            sem_post(&cheia); // Avisa o cozinheiro que todos os pratos estao sujos
        sem_post(&mutex);
        sem_post(&cozinhando);

        interesse[id] = false;
        sleep(1);
    }
}

void *cozinheiro(void *arg)
{
    int M = *((int *)arg);

    while (1) {
        sem_wait(&cheia); // Espera a travessa estar vazia
        sem_wait(&mutex);
        printf("\nCozinheiro está preparando comida...");
        pratos_sujos = 0;
        sem_post(&mutex);
        for (int i = 0; i < M; i++)
            sem_post(&cozinhando); // Enche a travessa
    }
}

int main()
{
    int N = 5;
    int M = 10;

    pthread_t tcozinheiro, tcanibais[N];

    sem_init(&mutex, 0, 1);
    sem_init(&cheia, 0, 0);
    sem_init(&fila_espera, 0, N);
    sem_init(&cozinhando, 0, M);

    interesse = (bool *)malloc(N * sizeof(bool));
    if (interesse == NULL) {
        fprintf(stderr, "Erro ao alocar memória para interesse.\n");
        exit(EXIT_FAILURE);
    }

    ParametrosCanibal params_canibal;
    params_canibal.M = M;
    params_canibal.N = N;

    pthread_create(&tcozinheiro, NULL, cozinheiro, (void *)&M);
    for (long int i = 0; i < N; i++) {
        interesse[i] = false;
        params_canibal.indice = i;
        pthread_create(&tcanibais[i], NULL, canibais, (void *)&params_canibal);
    }

    pthread_exit(NULL);

    return 0;
}
