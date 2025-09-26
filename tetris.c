#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILA 5   // Capacidade fixa da fila
#define MAX_PILHA 3  // Capacidade fixa da pilha

// --- Estrutura de Dados ---
struct Peca {
    char tipo; 
    int id;   
};

// --- Variaveis Globais (Controle de Estado) ---
struct Peca fila[MAX_FILA];
int frente = 0;         
int tras = 0;           
int tamanho_fila = 0;   

struct Peca pilha[MAX_PILHA];
int topo = -1;          

int contador_id = 0;    // Contador global para IDs unicos

// --- Funcoes de Utilitario ---

struct Peca gerarPeca() {
    struct Peca nova_peca;
    char tipos[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'}; 
    
    nova_peca.tipo = tipos[rand() % 7];
    nova_peca.id = contador_id++;
    
    return nova_peca;
}

// --- Funcoes da FILA CIRCULAR ---

int filaVazia() { return tamanho_fila == 0; }
int filaCheia() { return tamanho_fila == MAX_FILA; }

void enqueue(struct Peca nova_peca) {
    if (filaCheia()) return;
    
    fila[tras] = nova_peca;
    tras = (tras + 1) % MAX_FILA;
    tamanho_fila++;
}

struct Peca dequeue() {
    if (filaVazia()) {
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

void push(struct Peca peca) {
    if (pilhaCheia()) return;
    
    topo++;
    pilha[topo] = peca;
}

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
        printf("[Vazia]");
        return;
    }
    
    for (int i = topo; i >= 0; i--) {
        printf("[%c %d]", pilha[i].tipo, pilha[i].id);
    }
}

void exibirFila() {
    printf("Fila de Pecas: ");

    if (filaVazia()) {
        printf("[Vazia]");
        return;
    }

    int i = frente;
    int count = 0;
    while (count < tamanho_fila) {
        printf("[%c %d]", fila[i].tipo, fila[i].id);
        i = (i + 1) % MAX_FILA;
        count++;
    }
}

void exibirEstado() {
    printf("\n\n=== ESTADO ATUAL ===\n");
    exibirFila();
    printf(" "); // Separador visual
    exibirPilha();
    printf("\n======================\n");
}

// --- Funcoes de Acao do Jogador ---

// ACAO 1: Joga a peca da frente da fila e gera uma nova
void jogarPecaAcao() {
    if (filaVazia()) {
        printf("\n[ERRO] A fila esta vazia! Impossivel jogar.\n");
        return;
    }

    struct Peca peca_jogada = dequeue();
    printf("\nSUCESSO: Peca [%c %d] jogada no tabuleiro.\n", peca_jogada.tipo, peca_jogada.id);

    struct Peca nova = gerarPeca();
    enqueue(nova);
}

// ACAO 2: Move a peca da frente da fila para a pilha de reserva e gera uma nova
void reservarPecaAcao() {
    if (filaVazia()) {
        printf("\n[ERRO] A fila esta vazia! Nao ha pecas para reservar.\n");
        return;
    }
    if (pilhaCheia()) {
        printf("\n[ERRO] A Pilha de Reserva esta cheia (Max: %d)! Use uma peca reservada primeiro.\n", MAX_PILHA);
        return;
    }

    struct Peca peca_reservada = dequeue();
    push(peca_reservada);
    
    printf("\nSUCESSO: Peca [%c %d] reservada com sucesso.\n", peca_reservada.tipo, peca_reservada.id);

    struct Peca nova = gerarPeca();
    enqueue(nova);
}

// ACAO 3: Usa a peca do topo da pilha de reserva
void usarReservaAcao() {
    if (pilhaVazia()) {
        printf("\n[ERRO] A Pilha de Reserva esta vazia! Nao ha pecas para usar.\n");
        return;
    }

    struct Peca peca_usada = pop();
    printf("\nSUCESSO: Peca reservada [%c %d] utilizada no jogo.\n", peca_usada.tipo, peca_usada.id);
}

// ACAO 4: Troca a peca da frente da fila com o topo da pilha
void trocarPecaAtual() {
    if (filaVazia() || pilhaVazia()) {
        printf("\n[ERRO] A fila e/ou a pilha estao vazias. Impossivel trocar.\n");
        return;
    }
    
    struct Peca temp;
    
    // Armazena a peca da frente da fila
    temp = fila[frente];
    
    // Move a peca do topo da pilha para a frente da fila
    fila[frente] = pilha[topo];
    
    // Move a peca original da fila para o topo da pilha
    pilha[topo] = temp;
    
    printf("\nSUCESSO: Troca 1x1 realizada entre a frente da Fila e o Topo da Pilha.\n");
    // Nao ha mudanca no tamanho da fila/pilha ou nos ponteiros 'frente'/'topo'
}

// ACAO 5: Alterna as tres primeiras pecas da fila com as tres da pilha
void trocaMultipla() {
    if (tamanho_fila < 3 || topo < MAX_PILHA - 1) {
        printf("\n[ERRO] A Troca Multipla requer 3 pecas na Fila e 3 na Pilha. Impossivel realizar.\n");
        return;
    }
    
    struct Peca temp_fila[MAX_PILHA]; // Array temporario para as 3 pecas da fila
    
    // 1. Armazena as 3 primeiras pecas da Fila
    for (int i = 0; i < MAX_PILHA; i++) {
        temp_fila[i] = fila[(frente + i) % MAX_FILA];
    }
    
    // 2. Move as 3 pecas da Pilha para as 3 primeiras posicoes da Fila
    for (int i = 0; i < MAX_PILHA; i++) {
        fila[(frente + i) % MAX_FILA] = pilha[i];
    }
    
    // 3. Move as 3 pecas temporarias (originais da Fila) para a Pilha
    for (int i = 0; i < MAX_PILHA; i++) {
        pilha[i] = temp_fila[i];
    }
    
    printf("\nSUCESSO: Troca em Bloco realizada entre os 3 primeiros da Fila e as 3 da Pilha.\n");
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
        printf("4. Trocar peca da frente da fila com o topo da pilha (1x1)\n");
        printf("5. Troca multipla (3 da Fila x 3 da Pilha)\n");
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
            case 4:
                trocarPecaAtual();
                break;
            case 5:
                trocaMultipla();
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