
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define LINE_SIZE 10  // definição do tamanho da fila



/*
  Settado os valores globais do projeto
*/
int line[LINE_SIZE];
int begin = 0, end = 0;
int turn = 0;
int N;
int M;
int *flag;


/*
  Criada as primitivas do lock() e unlock() como requerida no caso
  de uso de semaforos binário (mutex)
*/
void lock(int id) {
    int other;
    flag[id] = 1;
    turn = id;
    
    for (other = 0; other < N+M; other++) {
        if (other != id) {
            while (flag[other] && turn == id);
        }
    }
}

void unlock(int id) {
    flag[id] = 0;
}


/*
  sem_t é um typedef 
  delcarando o o tipo das variávies abaixo como semáforos
*/
sem_t semaphore_full, semaphore_empty;


void *producer(void *arg) {
    int item;
    int id = *((int *)arg);  // identifica qual o id do produtor
    while (1) {
        item = rand() % 10;  // pega um valor aleatório de 0 a 9
        sem_wait(&semaphore_full);  // Aguarda espaço na fila
        
        lock(id);  // Seção crítica
        line[end] = item;  // coloca o valor (aleatório) no fim da fila
        int positionItem = end;  // pega o valor da posição que o item foi produzido
        end = (end + 1) % LINE_SIZE;  // vai para a próxima posição da fila e a torna circular
        printf("Item produzido: %d, adicionad a posicao: %d\n", item, positionItem);
        unlock(id);  // Seção crítica encerrada

        sem_post(&semaphore_empty);  // Sinaliza que a fila não está mais vazia
    }
}


void *consumer(void *arg) {
    int item;
    int id = *((int *)arg);
    while (1) {
        // Para travar (lock()) um semáforo ou esperar, se usa a função sem_wait
        sem_wait(&semaphore_empty);  // Aguarda itens na fila

        lock(id);  // Inicia a Seção Crítica
        item = line[begin];  // pega o valor que se encontra no inicio da fila
        int positionItem = begin;  // pega o valor da posição que o item foi consumido
        begin = (begin + 1) % LINE_SIZE;  // vai para a próxima posição da fila e a torna circular
        printf("Item consumido: %d, retirado da posicao: %d\n", item, positionItem);
        unlock(id);  // Encerra a Seção Crítica

        sem_post(&semaphore_full);  // Sinaliza que há espaço na fila
    }
}

int main(int n, char* arg[]) {
    
    N = atoi(arg[1]);  // primeiro argumento na linha de comando
    M = atoi(arg[2]);  // segundo argumento na linha de comando

    flag = calloc((N+M), sizeof(int));  // declara o tamanho da flag

    
    
    pthread_t producer_thread, consumer_thread;
    int id_producer = 0, id_consumer = 1;

    // Um semáforo é inicializado usando sem_init (para processos ou threads)
    sem_init(&semaphore_full, 0, LINE_SIZE);
    sem_init(&semaphore_empty, 0, 0);
    
    
    //adicionado
    int ids[N+M];
    
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&producer_thread, NULL, producer, &ids[i]);
    }
    for (int i = N; i < M+N; i++) {
        ids[i] = i;
        pthread_create(&consumer_thread, NULL, consumer, &ids[i]);
    }

    //end

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&semaphore_full);
    sem_destroy(&semaphore_empty);

    return 0;
}
