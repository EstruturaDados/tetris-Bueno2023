/*
    Trabalho de Estrutura de Dados - Gerenciador de Peças
    Aluno: Renan Felipe Timoteo Bueno   
    Data: 06/10/2025

    Objetivo:
    Implementar um sistema que gerencia peças de jogo usando uma Fila Circular
    para as peças ativas e uma Pilha para uma reserva estratégica. O programa
    deve permitir várias interações entre as duas estruturas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Definições Globais e Estruturas ---

// Constantes para os tamanhos das estruturas, facilita a manutenção.
#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

// A struct que representa uma única peça do jogo.
// Cada peça tem um tipo (letra) e um ID único para rastreamento.
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura da nossa Fila Circular.
// 'inicio' e 'fim' controlam a posição, e 'contador' nos diz
// quantas peças temos, o que é mais fácil do que calcular a diferença.
typedef struct {
    Peca pecas[TAMANHO_FILA];
    int inicio;
    int fim;
    int contador;
} FilaCircular;

// Estrutura da nossa Pilha de reserva.
// 'topo' indica o índice do último elemento inserido.
// Começa em -1 para indicar que está vazia.
typedef struct {
    Peca pecas[TAMANHO_PILHA];
    int topo;
} Pilha;

// Um contador global para garantir que cada peça tenha um ID único.
int proximo_id = 0;

// --- Protótipos das Funções ---

// Funções de gerenciamento de peças
Peca gerarPeca();

// Funções da Fila
void inicializarFila(FilaCircular *f);
void enqueue(FilaCircular *f, Peca p);
Peca dequeue(FilaCircular *f);

// Funções da Pilha
void inicializarPilha(Pilha *p);
void push(Pilha *p, Peca peca);
Peca pop(Pilha *p);

// Funções de interface e lógica do jogo
void exibirEstado(FilaCircular *f, Pilha *p);
void exibirMenu();


// --- Função Principal ---
int main() {
    // Inicializa o gerador de números aleatórios.
    // Faço isso uma vez no começo para que as peças sejam diferentes a cada execução.
    srand(time(NULL));

    FilaCircular fila_de_jogo;
    Pilha pilha_de_reserva;

    // Prepara as estruturas para o início do jogo
    inicializarFila(&fila_de_jogo);
    inicializarPilha(&pilha_de_reserva);

    int opcao;
    do {
        exibirEstado(&fila_de_jogo, &pilha_de_reserva);
        exibirMenu();

        printf("Opcao escolhida: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1: { // Jogar peça
                if (fila_de_jogo.contador > 0) {
                    Peca jogada = dequeue(&fila_de_jogo);
                    printf("Acao: Peca '%c' (ID %d) foi jogada.\n", jogada.nome, jogada.id);
                    // Gera uma nova peça para manter a fila cheia
                    enqueue(&fila_de_jogo, gerarPeca());
                } else {
                    printf("Acao: Fila vazia, impossivel jogar.\n");
                }
                break;
            }
            case 2: { // Reservar peça
                if (pilha_de_reserva.topo < TAMANHO_PILHA - 1) {
                    if (fila_de_jogo.contador > 0) {
                        Peca reservada = dequeue(&fila_de_jogo);
                        push(&pilha_de_reserva, reservada);
                        printf("Acao: Peca '%c' (ID %d) movida para a reserva.\n", reservada.nome, reservada.id);
                        enqueue(&fila_de_jogo, gerarPeca());
                    } else {
                        printf("Acao: Fila vazia, impossivel reservar.\n");
                    }
                } else {
                    printf("Acao: Pilha de reserva esta cheia!\n");
                }
                break;
            }
            case 3: { // Usar peça reservada
                if (pilha_de_reserva.topo > -1) {
                    Peca usada = pop(&pilha_de_reserva);
                    printf("Acao: Peca reservada '%c' (ID %d) foi usada.\n", usada.nome, usada.id);
                } else {
                    printf("Acao: Nenhuma peca na reserva para usar.\n");
                }
                break;
            }
            case 4: { // Trocar peça da frente com topo da pilha
                if (fila_de_jogo.contador > 0 && pilha_de_reserva.topo > -1) {
                    // Faço uma troca simples usando uma variável temporária.
                    Peca temp = fila_de_jogo.pecas[fila_de_jogo.inicio];
                    fila_de_jogo.pecas[fila_de_jogo.inicio] = pilha_de_reserva.pecas[pilha_de_reserva.topo];
                    pilha_de_reserva.pecas[pilha_de_reserva.topo] = temp;
                    printf("Acao: Troca simples realizada.\n");
                } else {
                    printf("Acao: E necessario ter pecas na fila e na pilha para trocar.\n");
                }
                break;
            }
            case 5: { // Troca múltipla
                if (fila_de_jogo.contador >= 3 && pilha_de_reserva.topo == 2) { // Pilha cheia
                    Peca temp;
                    for (int i = 0; i < 3; i++) {
                        // O cálculo do índice da fila precisa considerar a circularidade.
                        int indice_fila = (fila_de_jogo.inicio + i) % TAMANHO_FILA;
                        
                        // Troca a peça da fila com a peça da pilha
                        temp = fila_de_jogo.pecas[indice_fila];
                        fila_de_jogo.pecas[indice_fila] = pilha_de_reserva.pecas[i];
                        pilha_de_reserva.pecas[i] = temp;
                    }
                    printf("Acao: Troca multipla realizada entre 3 pecas.\n");
                } else {
                    printf("Acao: Para a troca multipla, a fila deve ter no minimo 3 pecas e a pilha deve estar cheia.\n");
                }
                break;
            }
            case 0:
                printf("Encerrando o gerenciador de pecas...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
        printf("\n");

    } while (opcao != 0);

    return 0;
}


// --- Implementação das Funções Auxiliares ---

// Gera uma nova peça aleatória com um ID sequencial.
Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'}; // Mais tipos para variedade
    p.nome = tipos[rand() % 7];
    p.id = proximo_id++;
    return p;
}

// Prepara a fila para o início, zerando os controles e preenchendo com peças.
void inicializarFila(FilaCircular *f) {
    f->inicio = 0;
    f->fim = 0;
    f->contador = 0;
    // Preenche a fila com as 5 peças iniciais.
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue(f, gerarPeca());
    }
}

// Adiciona uma peça no fim da fila.
void enqueue(FilaCircular *f, Peca p) {
    if (f->contador < TAMANHO_FILA) {
        f->pecas[f->fim] = p;
        // A "mágica" da fila circular está aqui, no operador de módulo.
        f->fim = (f->fim + 1) % TAMANHO_FILA;
        f->contador++;
    }
}

// Remove e retorna a peça do início da fila.
Peca dequeue(FilaCircular *f) {
    Peca p = {' ', -1}; // Peça vazia para caso de erro
    if (f->contador > 0) {
        p = f->pecas[f->inicio];
        f->inicio = (f->inicio + 1) % TAMANHO_FILA;
        f->contador--;
    }
    return p;
}

// Prepara a pilha, definindo o topo como -1 (vazio).
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Adiciona uma peça no topo da pilha.
void push(Pilha *p, Peca peca) {
    if (p->topo < TAMANHO_PILHA - 1) {
        p->topo++;
        p->pecas[p->topo] = peca;
    }
}

// Remove e retorna a peça do topo da pilha.
Peca pop(Pilha *p) {
    Peca peca = {' ', -1};
    if (p->topo > -1) {
        peca = p->pecas[p->topo];
        p->topo--;
    }
    return peca;
}

// Mostra o menu de opções para o jogador.
void exibirMenu() {
    printf("Opcoes disponiveis:\n");
    printf(" 1. Jogar peca da frente da fila\n");
    printf(" 2. Enviar peca da fila para a pilha de reserva\n");
    printf(" 3. Usar peca da pilha de reserva\n");
    printf(" 4. Trocar peca da frente da fila com o topo da pilha\n");
    printf(" 5. Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
    printf(" 0. Sair\n\n");
}

// Função para desenhar o estado atual do jogo no console.
void exibirEstado(FilaCircular *f, Pilha *p) {
    printf("========================================================\n");
    printf("Estado Atual:\n\n");

    // Mostra a Fila
    printf("Fila de Pecas -> ");
    if (f->contador == 0) {
        printf("[Vazia]");
    } else {
        for (int i = 0; i < f->contador; i++) {
            int index = (f->inicio + i) % TAMANHO_FILA;
            printf("[%c %d] ", f->pecas[index].nome, f->pecas[index].id);
        }
    }
    printf("\n");

    // Mostra a Pilha
    printf("Pilha de Reserva (Topo -> Base) -> ");
    if (p->topo == -1) {
        printf("[Vazia]");
    } else {
        // Imprimo do topo para a base, como é o padrão da pilha.
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
        }
    }
    printf("\n");
    printf("========================================================\n\n");
}