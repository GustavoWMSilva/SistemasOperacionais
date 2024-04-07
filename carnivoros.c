#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_CANIBAIS 5
#define M 10


int pratos_sujos = 0;
sem_t mutex, cheia, cozinhando;
int fila[MAX_CANIBAIS];
int inicio = 0, fim = 0;

void *canibais(void *arg)
{
    long id = (long)arg;

    while (1) {
        sem_wait(&mutex);
        fila[fim] = id;
        fim = (fim + 1) % MAX_CANIBAIS;
        printf("\nCanibal %ld chegou. Pratos sujos: %d", id, pratos_sujos);
        sem_post(&mutex);

        sem_wait(&cozinhando);
        sem_wait(&mutex);
        printf("\nCanibal %ld comeu. Pratos sujos: %d", id, pratos_sujos);
        
        pratos_sujos++;
        if (pratos_sujos == M)
            sem_post(&cheia); // Avisa o cozinheiro que todos os pratos estao sujos
        sem_post(&mutex);
        sem_post(&cozinhando);
        sleep(1);
    }
}

void *cozinheiro(void *arg)
{
    //int M = *((int *)arg);

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
    //int M = 10;

    pthread_t tcozinheiro, tcanibais[N];

    sem_init(&mutex, 0, 1);
    sem_init(&cheia, 0, 0);
    sem_init(&cozinhando, 0, M);

    for (long int i = 0; i < N; i++) {
        pthread_create(&tcanibais[i], NULL, canibais, (void *)i);
    }

    //pthread_create(&tcozinheiro, NULL, cozinheiro, (void *)&M);
    pthread_create(&tcozinheiro, NULL, cozinheiro, (void *)0);

    pthread_exit(NULL);

    return 0;
}
