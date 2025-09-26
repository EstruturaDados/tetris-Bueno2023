#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILA 5   // Capacidade fixa da fila
#define MAX_PILHA 3  // Capacidade fixa da pilha

// --- Definição da Struct ---
struct Peca {
    char tipo; // Ex: 'I', 'O', 'T', 'L'
    int id;   // Identificador unico
};

// --- Variaveis Globais (Controle da Fila e Pilha) ---
struct Peca fila[MAX_FILA];
int frente = 0;         // Indice da proxima remocao (Fila)
int tras = 0;           // Indice da proxima insercao (Fila)
int tamanho_fila = 0;   // Contador de elementos na fila

struct Peca pilha[MAX_PILHA];
int topo = -1;          // Indice do topo da pilha (-1 = vazia)

int contador_id = 0;    // Contador global para gerar IDs unicos

// --- Funcoes de Utilitario ---

struct Peca gerarPeca() {
    struct Peca nova_peca;
    char tipos[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'}; // Tipos de pecas
    
    // Sorteia o tipo
    nova_peca.tipo = tipos[rand() % 7];
    
    // Atribui ID unico e avanca o contador
    nova_peca.id = contador_id++;
    
    return nova_peca;
}

// --- Funcoes da FILA CIRCULAR ---

int filaVazia() { return tamanho_fila == 0; }
int filaCheia() { return tamanho_fila == MAX_FILA; }

// Insere uma peca no final da fila (Enqueue)
void enqueue(struct Peca nova_peca) {
    if (filaCheia()) {
        // Nao deve acontecer no loop principal, mas e uma boa pratica de programacao
        printf("\n[ALERTA] Fila cheia. Impossivel adicionar nova peca.\n");
        return;
    }
    
    fila[tras] = nova_peca;
    tras = (tras + 1) % MAX_FILA;
    tamanho_fila++;
}

// Remove a peca da frente da fila (Dequeue)
struct Peca dequeue() {
    if (filaVazia()) {
        // Retorna uma peca invalida em caso de erro
        struct Peca peca_erro = {'X', -1}; 
        return peca_erro;
    }

    struct Peca peca_removida = fila[frente];
    
    frente = (frente + 1) % MAX_FILA;
    tamanho_fila--;
    
    return peca_removida;
}

// --- Funcoes da PILHA ---

int pilhaVazia() { return topo == -1; }
int pilhaCheia() { return topo == MAX_PILHA - 1; }

// Adiciona uma peca ao topo da pilha (Push)
void push(struct Peca peca) {
    if (pilhaCheia()) {
        printf("\n[ALERTA] Pilha de Reserva cheia! Nao e possivel reservar mais pecas.\n");
        return;
    }
    
    topo++;
    pilha[topo] = peca;
}

// Remove a peca do topo da pilha (Pop)
struct Peca pop() {
    if (pilhaVazia()) {
        struct Peca peca_erro = {'X', -1};
        return peca_erro;
    }

    struct Peca peca_removida = pilha[topo];
    topo--;
    
    return peca_removida;
}

// --- Funcoes de Exibicao ---

void exibirPilha() {
    printf("Pilha de Reserva (Topo -> Base): ");
    if (pilhaVazia()) {
        printf("[Vazia]\n");
        return;
    }
    
    for (int i = topo; i >= 0; i--) {
        printf("[%c %d]", pilha[i].tipo, pilha[i].id);
    }
    printf("\n");
}

void exibirFila() {
    printf("Fila de Pecas [Total: %d/%d]: ", tamanho_fila, MAX_FILA);

    if (filaVazia()) {
        printf("[Vazia]\n");
        return;
    }

    int i = frente;
    int count = 0;
    while (count < tamanho_fila) {
        printf("[%c %d]", fila[i].tipo, fila[i].id);
        i = (i + 1) % MAX_FILA;
        count++;
    }
    printf("\n");
}

void exibirEstado() {
    printf("\n=== ESTADO ATUAL ===\n");
    exibirFila();
    exibirPilha();
    printf("======================\n");
}

// --- Funcoes de Acao do Jogador ---

// ACAO 1: Joga a peca da frente da fila e gera uma nova
void jogarPecaAcao() {
    if (filaVazia()) {
        printf("\n[ERRO] A fila esta vazia! Impossivel jogar.\n");
        return;
    }

    struct Peca peca_jogada = dequeue();
    printf("\nSUCESSO: Peca [%c %d] jogada no tabuleiro!\n", peca_jogada.tipo, peca_jogada.id);

    // Regra: Apos jogar uma peca, uma nova peca e gerada para manter a fila cheia
    struct Peca nova = gerarPeca();
    enqueue(nova);
    printf("[Aviso] Nova peca [%c %d] gerada e adicionada a fila.\n", nova.tipo, nova.id);
}

// ACAO 2: Move a peca da frente da fila para a pilha de reserva
void reservarPecaAcao() {
    if (filaVazia()) {
        printf("\n[ERRO] A fila esta vazia! Nao ha pecas para reservar.\n");
        return;
    }
    if (pilhaCheia()) {
        printf("\n[ERRO] A Pilha de Reserva esta cheia (Max: %d)! Use uma peca reservada primeiro.\n", MAX_PILHA);
        return;
    }

    // 1. Remove da Fila
    struct Peca peca_reservada = dequeue();
    
    // 2. Adiciona a Pilha
    push(peca_reservada);
    
    printf("\nSUCESSO: Peca [%c %d] reservada com sucesso!\n", peca_reservada.tipo, peca_reservada.id);

    // Regra: Uma nova peca e gerada para manter a fila cheia
    struct Peca nova = gerarPeca();
    enqueue(nova);
    printf("[Aviso] Nova peca [%c %d] gerada e adicionada a fila.\n", nova.tipo, nova.id);
}

// ACAO 3: Usa a peca do topo da pilha de reserva
void usarReservaAcao() {
    if (pilhaVazia()) {
        printf("\n[ERRO] A Pilha de Reserva esta vazia! Nao ha pecas para usar.\n");
        return;
    }

    // Remove da Pilha
    struct Peca peca_usada = pop();
    
    printf("\nSUCESSO: Peca reservada [%c %d] utilizada no jogo!\n", peca_usada.tipo, peca_usada.id);
    // Regra: Nenhuma nova peca e gerada nesta acao
}

// --- Funcao Principal ---
int main() {
    int opcao;

    srand(time(NULL));

    // --- Inicializacao do Jogo ---
    // A regra exige que a fila comece cheia (5 elementos)
    printf("--- INICIALIZANDO JOGO ---\n");
    for (int i = 0; i < MAX_FILA; i++) {
        enqueue(gerarPeca());
    }
    printf("Fila inicializada com %d pecas. Pilha de reserva vazia.\n", MAX_FILA);

    // Loop principal do menu
    do {
        exibirEstado();
        
        printf("\n--- OPCOES DE ACAO ---\n");
        printf("1. Jogar peca (Remove da Fila)\n");
        printf("2. Reservar peca (Fila -> Pilha)\n");
        printf("3. Usar peca reservada (Remove da Pilha)\n");
        printf("0. Sair\n");
        printf("Escolha uma acao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("\nOpcao invalida. Digite um numero.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1:
                jogarPecaAcao();
                break;
            case 2:
                reservarPecaAcao();
                break;
            case 3:
                usarReservaAcao();
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