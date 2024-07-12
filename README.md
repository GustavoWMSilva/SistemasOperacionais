# README.md

## Sistemas Operacionais - Trabalho 1

Este repositório contém a implementação das soluções para os problemas "Produtores e Consumidores" e "Jantar dos Canibais", propostos no primeiro trabalho da disciplina de Sistemas Operacionais. As soluções foram desenvolvidas utilizando múltiplas threads e técnicas de sincronização.

### Objetivos

1. Utilizar programação e comunicação entre múltiplas threads.
2. Praticar técnicas de sincronização de threads.

### Problema 1: Produtores e Consumidores

Em um sistema, existe uma fila compartilhada por múltiplas threads. Algumas threads produzem itens e os adicionam na fila, enquanto outras retiram e consomem itens dessa fila. A solução desenvolvida aborda os seguintes pontos:

- A fila é modelada por um vetor com referências às posições atuais (início e fim da fila), definindo uma fila circular.
- As posições são acessadas de maneira ordenada por meio de uma seção crítica.
- A solução evita que threads produtoras adicionem mais itens na fila quando estiver cheia.
- A solução evita que threads consumidoras retirem itens da fila quando estiver vazia.

#### Arquivo: `produtor_consumidor.c`

### Problema 2: Jantar dos Canibais

Neste problema, um grupo de N canibais se serve de uma travessa que comporta M porções. Quando um canibal quer comer, ele se serve da travessa, a menos que ela esteja vazia. Nesse caso, o canibal acorda o cozinheiro e espera até que ele coloque mais M porções na travessa. A solução inclui:

- Modelagem dos canibais e do cozinheiro como threads.
- Sincronização das ações dos canibais e do cozinheiro para evitar deadlocks e garantir que o cozinheiro seja acordado apenas quando a travessa estiver vazia.

#### Arquivo: `carnivos.c`

### Sincronização

Ambas as soluções utilizam semáforos contadores e mutex (ou semáforo binário) para sincronização. Para a implementação do mutex, foi utilizado o algoritmo de Peterson para as primitivas `lock()` e `unlock()`. Os semáforos contadores são deixados como na solução original.

### Como Compilar e Executar

#### Compilação

Para compilar os arquivos, utilize o comando `gcc`:

```sh
gcc produtor_consumidor.c -o produtor_consumidor -lpthread
gcc carnivos.c -o carnivos -lpthread
```

#### Execução

Para executar o programa de produtores e consumidores:

```sh
./produtor_consumidor <N> <M>
```

- `<N>`: Número de produtores.
- `<M>`: Número de consumidores.

Para executar o programa de jantar dos canibais:

```sh
./carnivos <N> <M>
```

- `<N>`: Número de canibais.
- `<M>`: Porções por reabastecimento.

### Dependências

- GCC (GNU Compiler Collection)
- Biblioteca pthread para suporte a threads.

### Contato

Para mais informações, entre em contato com os desenvolvedores.

---

Este trabalho foi desenvolvido como parte da disciplina de Sistemas Operacionais, sob a orientação do professor Sergio.
