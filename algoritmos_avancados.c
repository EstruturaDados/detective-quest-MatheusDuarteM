#include <stdio.h>  // Para operações de entrada e saída (printf, scanf)
#include <stdlib.h> // Para alocação dinâmica (malloc, free, exit)
#include <string.h> // Para comparação de strings (strcmp)

typedef struct Sala {
    char* nome;             // Nome do cômodo (ex: "Cozinha")
    char* pista;            // Pista opcional encontrada neste cômodo
    struct Sala* esquerda;  // Ponteiro para o cômodo à esquerda
    struct Sala* direita;   // Ponteiro para o cômodo à direita
} Sala;

/**
 * @brief Define a estrutura de um nó da Árvore Binária de Busca (BST).
 * Cada nó armazena uma pista coletada.
 */
typedef struct PistaNode {
    char* pista;
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;



Sala* criarSala(char* nome, char* pista) {
    // 1. Aloca memória
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));

    // 2. Verifica se a alocação foi bem-sucedida
    if (novaSala == NULL) {
        printf("Erro fatal: Falha ao alocar memória para a sala '%s'!\n", nome);
        exit(1);
    }

    // 3. Inicializa os valores da sala
    novaSala->nome = nome;
    novaSala->pista = pista; // Adiciona a pista
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}


PistaNode* inserirPista(PistaNode* raiz, char* pista) {
    // 1. Caso Base: Se a árvore/sub-árvore está vazia, cria o novo nó aqui.
    if (raiz == NULL) {
        PistaNode* novoNode = (PistaNode*)malloc(sizeof(PistaNode));
        if (novoNode == NULL) {
            printf("Erro fatal: Falha ao alocar memória para a pista!\n");
            exit(1);
        }
        novoNode->pista = pista;
        novoNode->esquerda = NULL;
        novoNode->direita = NULL;
        return novoNode; // Este novo nó é a nova raiz
    }

    // 2. Caso Recursivo: Compara a nova pista com a pista na raiz
    int comparacao = strcmp(pista, raiz->pista);

    if (comparacao < 0) {
        // Nova pista vem antes (alfabeticamente), vai para a esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        // Nova pista vem depois (alfabeticamente), vai para a direita
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se comparacao == 0, a pista já existe, não faz nada.

    // 3. Retorna a raiz (que pode ter sido atualizada nas chamadas recursivas)
    return raiz;
}

void exibirPistas(PistaNode* raiz) {
    // Caso base da recursão
    if (raiz == NULL) {
        return;
    }

    // 1. Visita a sub-árvore esquerda (pistas que vêm antes)
    exibirPistas(raiz->esquerda);

    // 2. Visita a raiz (imprime a pista atual)
    printf("- %s\n", raiz->pista);

    // 3. Visita a sub-árvore direita (pistas que vêm depois)
    exibirPistas(raiz->direita);
}



void explorarSalasComPistas(Sala* salaInicial, PistaNode** raizPistas) {
    Sala* salaAtual = salaInicial; // O jogador começa na sala inicial
    char escolha;

    while (salaAtual != NULL) {
        printf("\n===================================\n");
        printf("Você está em: %s\n", salaAtual->nome);
        printf("===================================\n");

        // 1. Lógica de Coleta de Pista
        if (salaAtual->pista != NULL) {
            printf("!! Você encontrou uma pista: %s\n", salaAtual->pista);
            
            // Insere a pista na BST e ATUALIZA a raiz da BST em main()
            *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
            
            // Remove a pista da sala para não ser coletada novamente
            salaAtual->pista = NULL;
        }

        // 2. Apresenta as opções de caminho
        // (Não há mais detecção automática de nó-folha, conforme requisito)
        printf("Escolha seu caminho:\n");
        if (salaAtual->esquerda != NULL) {
            printf(" (e) Ir para a Esquerda\n");
        }
        if (salaAtual->direita != NULL) {
            printf(" (d) Ir para a Direita\n");
        }
        printf(" (s) Sair do Jogo e ver pistas\n");
        printf("Sua escolha: ");

        // 3. Lê a escolha do jogador
        scanf(" %c", &escolha);

        // 4. Processa a escolha
        switch (escolha) {
            case 'e':
            case 'E':
                if (salaAtual->esquerda != NULL) {
                    salaAtual = salaAtual->esquerda;
                } else {
                    printf("\n-> Caminho bloqueado! Não há passagem por aqui.\n");
                }
                break;

            case 'd':
            case 'D':
                if (salaAtual->direita != NULL) {
                    salaAtual = salaAtual->direita;
                } else {
                    printf("\n-> Caminho bloqueado! Não há passagem por aqui.\n");
                }
                break;

            case 's':
            case 'S':
                printf("\nSaindo da mansão para analisar as pistas...\n");
                return; // Encerra a função e o loop, voltando para main()

            default:
                printf("\n-> Opção inválida! Tente 'e', 'd' ou 's'.\n");
                break;
        }
    }
}


int main() {
    // 1. Inicializa a Árvore de Pistas (BST)
    PistaNode* raizPistas = NULL;

    // 2. Montagem do Mapa da Mansão (com Pistas)
    // Nível 0 (Raiz)
    Sala* hall = criarSala("Hall de Entrada", NULL);

    // Nível 1
    hall->esquerda = criarSala("Sala de Estar", "Um diário antigo");
    hall->direita = criarSala("Biblioteca", "Carta com um selo estranho");

    // Nível 2 (Ala Esquerda)
    hall->esquerda->esquerda = criarSala("Sala de Jantar", NULL);
    hall->esquerda->direita = criarSala("Jardim de Inverno", "Pétala de uma flor rara");

    // Nível 2 (Ala Direita)
    hall->direita->esquerda = criarSala("Escritório", "Chave enferrujada");
    hall->direita->direita = criarSala("Quarto Principal", NULL);

    // Nível 3 (Folhas)
    hall->esquerda->esquerda->esquerda = criarSala("Cozinha", "Frasco de veneno (vazio)");
    hall->direita->esquerda->direita = criarSala("Passagem Secreta", "Foto rasgada");

    // 3. Início do Jogo (Exploração)
    printf("--- Bem-vindo ao Detective Quest ---\n");
    printf("Explore a mansão em busca de pistas.\n");

    // Inicia a exploração, passando o ponteiro da raiz do mapa
    // e o ENDEREÇO do ponteiro da raiz das pistas (&raizPistas)
    explorarSalasComPistas(hall, &raizPistas);

    // 4. Exibição das Pistas Coletadas (Após o jogador sair)
    printf("\n\n--- PISTAS COLETADAS (EM ORDEM ALFABÉTICA) ---\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        // Chama a função de exibição em-ordem da BST
        exibirPistas(raizPistas);
    }
    printf("------------------------------------------------\n");



    return 0;
}
