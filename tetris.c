#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILA 5 // Capacidade maxima da fila de pecas

// --- Definição da Struct ---
// Estrutura para representar uma peça do Tetris
struct Peca {
    char tipo; // Ex: 'I', 'O', 'T', 'L'
    int id;   // Identificador unico
};

// --- Variaveis Globais (Controle da Fila e IDs) ---
struct Peca fila[MAX_FILA];
int frente = 0;         // Indice para a proxima remocao (dequeue)
int tras = 0;           // Indice para a proxima insercao (enqueue)
int tamanho_fila = 0;   // Numero atual de elementos na fila
int contador_id = 0;    // Contador global para gerar IDs unicos

// --- Funcoes de Utilitario ---

// Verifica se a fila esta vazia
int filaVazia() {
    return tamanho_fila == 0;
}

// Verifica se a fila esta cheia
int filaCheia() {
    return tamanho_fila == MAX_FILA;
}

// Gera uma peca aleatoria com um ID unico
struct Peca gerarPeca() {
    struct Peca nova_peca;
    char tipos[] = {'I', 'O', 'T', 'L'}; // Pecas disponiveis
    
    // Sorteia o tipo
    nova_peca.tipo = tipos[rand() % 4];
    
    // Atribui ID e incrementa o contador global
    nova_peca.id = contador_id++;
    
    return nova_peca;
}

// Exibe o estado atual da fila de forma clara
void exibirFila() {
    printf("\nCuradoria de Pecas [Total: %d/%d]\n", tamanho_fila, MAX_FILA);
    printf("Fila: ");

    if (filaVazia()) {
        printf("[Vazia]\n");
        return;
    }

    int i = frente;
    int count = 0;
    while (count < tamanho_fila) {
        printf("[%c %d]", fila[i].tipo, fila[i].id);
        i = (i + 1) % MAX_FILA; // Avanca de forma circular
        count++;
    }
    printf("\n");
}

// --- Funcoes de Manipulacao da Fila ---

// 1. Remove a peca da frente da fila (Dequeue)
void jogarPeca() {
    if (filaVazia()) {
        printf("\nERRO: A fila de pecas esta vazia! Nao ha pecas para jogar.\n");
        return;
    }

    struct Peca peca_jogada = fila[frente];
    
    // Avanca o ponteiro da frente de forma circular
    frente = (frente + 1) % MAX_FILA;
    tamanho_fila--;
    
    printf("\nSUCESSO: Peca [%c %d] jogada! (Removida da frente)\n", peca_jogada.tipo, peca_jogada.id);
}

// 2. Insere uma nova peca no final da fila (Enqueue)
void inserirPeca() {
    if (filaCheia()) {
        printf("\nERRO: A fila de pecas esta cheia! Jogue uma peca primeiro.\n");
        return;
    }

    struct Peca nova_peca = gerarPeca();
    
    // Insere a nova peca na posicao 'tras'
    fila[tras] = nova_peca;
    
    // Avanca o ponteiro 'tras' de forma circular
    tras = (tras + 1) % MAX_FILA;
    tamanho_fila++;
    
    printf("\nSUCESSO: Nova peca [%c %d] inserida no final da fila.\n", nova_peca.tipo, nova_peca.id);
}


// --- Funcao Principal (Fluxo do Programa) ---
int main() {
    int opcao;

    // Inicializa o gerador de numeros aleatorios para sortear o tipo da peca
    srand(time(NULL));

    // --- Inicializacao da Fila (Requisito: 5 elementos) ---
    printf("--- INICIALIZANDO FILA DE PECAS ---\n");
    for (int i = 0; i < MAX_FILA; i++) {
        inserirPeca();
    }
    printf("\nFila inicializada com sucesso!\n");
    
    // Loop principal do menu
    do {
        exibirFila();
        
        printf("\n--- OPCOES DE ACAO ---\n");
        printf("1. Jogar peca (Dequeue)\n");
        printf("2. Inserir nova peca (Enqueue)\n");
        printf("0. Sair\n");
        printf("Escolha uma acao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("\nOpcao invalida. Digite um numero.\n");
            while (getchar() != '\n'); // Limpa o buffer de entrada
            continue;
        }

        switch (opcao) {
            case 1:
                jogarPeca();
                break;
            case 2:
                inserirPeca();
                break;
            case 0:
                printf("\nSaindo do simulador. Ate mais!\n");
                break;
            default:
                printf("\nComando desconhecido. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}