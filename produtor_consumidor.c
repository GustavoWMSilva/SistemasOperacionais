#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TAMANHO_FILA 10

int fila[TAMANHO_FILA];
int inicio = 0, fim = 0;
int vez = 0;
int flag[2] = {0};

void lock(int id) {
    int outro = 1 - id;
    flag[id] = 1;
    vez = outro;
    while (flag[outro] && vez == outro);
}

void unlock(int id) {
    flag[id] = 0;
}

sem_t semaforo_mutex, semaforo_cheio, semaforo_vazio;


void *produtor(void *arg) {
    int item;
    int id = *((int *)arg);
    while (1) {
        item = rand() % 10; 
        sem_wait(&semaforo_cheio); // Aguarda espaço na fila
        lock(id);//Secao critica
        fila[fim] = item;
        fim = (fim + 1) % TAMANHO_FILA;
        printf("Item produzido: %d, posicao: %d\n", item, fim);
        unlock(id);//Secao critica
        sem_post(&semaforo_vazio); // Sinaliza que a fila não está mais vazia
    }
}

void *consumidor(void *arg) {
    int item;
    int id = *((int *)arg);
    while (1) {
        sem_wait(&semaforo_vazio); // Aguarda itens na fila
        lock(id);//Secao critica
        item = fila[inicio];
        inicio = (inicio + 1) % TAMANHO_FILA;
        printf("Item consumido: %d\n", item);
        unlock(id);//Secao critica
        sem_post(&semaforo_cheio); // Sinaliza que há espaço na fila
    }
}

int main() {
    pthread_t produtor_thread, consumidor_thread;
    int id_produtor = 0, id_consumidor = 1;

    sem_init(&semaforo_cheio, 0, TAMANHO_FILA);
    sem_init(&semaforo_vazio, 0, 0);

    pthread_create(&produtor_thread, NULL, produtor, &id_produtor);
    pthread_create(&consumidor_thread, NULL, consumidor, &id_consumidor);

    pthread_join(produtor_thread, NULL);
    pthread_join(consumidor_thread, NULL);

    sem_destroy(&semaforo_cheio);
    sem_destroy(&semaforo_vazio);

    return 0;
}
