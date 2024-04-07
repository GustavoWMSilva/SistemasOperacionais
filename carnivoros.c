#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define M 10


int pratos_sujos = 0;
sem_t mutex, cheia, cozinhando, travessa_vazia;

void *canibais(void *arg)
{
    long int i = (long int)arg;

    while (1) {
        sem_wait(&cozinhando);
        sem_wait(&mutex);
        printf("\nCanibal %ld comeu um prato. Pratos sujos: %d", i, pratos_sujos);
        pratos_sujos++;
        if (pratos_sujos == M)
            sem_post(&cheia); // Avisa o cozinheiro que a travessa está cheia
        sem_post(&mutex);
    }
}

void *cozinheiro(void *arg)
{
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
    pthread_t tcozinheiro, tcanibais[N];
    
    sem_init(&mutex, 0, 1);
    sem_init(&cheia, 0, 0);
    sem_init(&cozinhando, 0, M);

    pthread_create(&tcozinheiro, NULL, cozinheiro, NULL);
    for (long int i = 0; i < N; i++)
        pthread_create(&tcanibais[i], NULL, canibais, (void *)(i+1));

    pthread_exit(NULL);
    
    return 0;
}