#include <stdio.h>  // Para operações de entrada e saída (printf, scanf)
#include <stdlib.h> // Para alocação dinâmica (malloc, free, exit)


typedef struct Sala {
    char* nome;             // Nome do cômodo (ex: "Cozinha")
    struct Sala* esquerda;  // Ponteiro para o cômodo à esquerda
    struct Sala* direita;   // Ponteiro para o cômodo à direita
} Sala;

Sala* criarSala(char* nome) {
    // 1. Aloca memória do tamanho da struct Sala
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));

    // 2. Verifica se a alocação foi bem-sucedida
    if (novaSala == NULL) {
        printf("Erro fatal: Falha ao alocar memória para a sala '%s'!\n", nome);
        exit(1); // Encerra o programa se não houver memória
    }

    // 3. Inicializa os valores da sala
    novaSala->nome = nome;
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    // 4. Retorna o ponteiro para a sala criada
    return novaSala;
}

void explorarSalas(Sala* salaInicial) {
    Sala* salaAtual = salaInicial; // O jogador começa na sala inicial
    char escolha;

    // O loop continua enquanto o jogador estiver em uma sala válida
    while (salaAtual != NULL) {
        printf("\n===================================\n");
        printf("Você está em: %s\n", salaAtual->nome);
        printf("===================================\n");

        // 1. Verifica se é um nó-folha (sem saídas)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Este cômodo não tem mais saídas. Você encontrou um beco sem saída.\n");
            printf("Fim da exploração neste caminho.\n");
            break; // Termina o loop de exploração
        }

        // 2. Apresenta as opções de caminho
        printf("Escolha seu caminho:\n");
        if (salaAtual->esquerda != NULL) {
            printf(" (e) Ir para a Esquerda\n");
        }
        if (salaAtual->direita != NULL) {
            printf(" (d) Ir para a Direita\n");
        }
        printf(" (s) Sair do Jogo\n");
        printf("Sua escolha: ");

        // 3. Lê a escolha do jogador
        // O espaço antes de "%c" é importante para consumir
        // quebras de linha (Enter) de leituras anteriores.
        scanf(" %c", &escolha);

        // 4. Processa a escolha
        switch (escolha) {
            case 'e':
            case 'E':
                if (salaAtual->esquerda != NULL) {
                    salaAtual = salaAtual->esquerda; // Move o jogador para a esquerda
                } else {
                    printf("\n-> Caminho bloqueado! Não há passagem por aqui.\n");
                }
                break;

            case 'd':
            case 'D':
                if (salaAtual->direita != NULL) {
                    salaAtual = salaAtual->direita; // Move o jogador para a direita
                } else {
                    printf("\n-> Caminho bloqueado! Não há passagem por aqui.\n");
                }
                break;

            case 's':
            case 'S':
                printf("\nSaindo do Detective Quest. Até logo!\n");
                return; // Encerra a função e o loop

            default:
                printf("\n-> Opção inválida! Tente 'e', 'd' ou 's'.\n");
                break;
        }
    }
}

/**
 * @brief Função principal (main).
 *
 * Monta o mapa inicial (a árvore da mansão) de forma automática
 * e, em seguida, dá início à exploração pelo jogador.
 */
int main() {
    // 1. Montagem do Mapa da Mansão (Criação da Árvore)
    // A árvore é criada aqui, de forma estática, usando criarSala()

    // Nível 0 (Raiz)
    Sala* hall = criarSala("Hall de Entrada");

    // Nível 1 (Filhos do Hall)
    hall->esquerda = criarSala("Sala de Estar");
    hall->direita = criarSala("Biblioteca");

    // Nível 2 (Ala Esquerda)
    hall->esquerda->esquerda = criarSala("Sala de Jantar");
    hall->esquerda->direita = criarSala("Jardim de Inverno");

    // Nível 2 (Ala Direita)
    hall->direita->esquerda = criarSala("Escritório");
    hall->direita->direita = criarSala("Quarto Principal");

    // Nível 3 (Folhas)
    hall->esquerda->esquerda->esquerda = criarSala("Cozinha");
    hall->direita->esquerda->direita = criarSala("Passagem Secreta");


    // 2. Início do Jogo (Exploração)
    printf("--- Bem-vindo ao Detective Quest ---\n");
    printf("Explore a mansão em busca de pistas.\n");

    // Inicia a exploração a partir do Hall de Entrada
    explorarSalas(hall);

    return 0; // Indica que o programa terminou com sucesso
}
