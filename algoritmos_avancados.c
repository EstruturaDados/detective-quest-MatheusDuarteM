#include <stdio.h>  // Para operações de entrada e saída (printf, scanf)
#include <stdlib.h> // Para alocação dinâmica (malloc, free, exit)
#include <string.h> // Para comparação e manipulação de strings (strcmp, strcpy)

#define HASH_TABLE_SIZE 11


typedef struct Sala {
    char* nome;
    char* pista;
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;


typedef struct PistaNode {
    char* pista;
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;


typedef struct HashEntry {
    char* pista;
    char* suspeito;
    struct HashEntry* next;
} HashEntry;

typedef struct HashTable {
    HashEntry** items; // Um array de ponteiros para HashEntry (os "buckets")
    int size;
} HashTable;


unsigned int hashFunction(char* pista, int size) {
    unsigned long hash = 5381;
    int c;

    while ((c = *pista++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    
    return hash % size;
}


HashTable* criarHashTable(int size) {
    // Aloca a estrutura da tabela
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (table == NULL) {
        printf("Erro fatal: Falha ao alocar memória para a Tabela Hash!\n");
        exit(1);
    }
    
    table->size = size;
    
    // Aloca o array de ponteiros (buckets)
    table->items = (HashEntry**)calloc(table->size, sizeof(HashEntry*));
    if (table->items == NULL) {
        printf("Erro fatal: Falha ao alocar memória para os itens da Hash!\n");
        free(table);
        exit(1);
    }
    
    return table;
}

void inserirNaHash(HashTable* table, char* pista, char* suspeito) {
    // 1. Calcula o índice
    unsigned int index = hashFunction(pista, table->size);
    
    // 2. Aloca memória para a nova entrada
    HashEntry* novaEntry = (HashEntry*)malloc(sizeof(HashEntry));
    if (novaEntry == NULL) {
        printf("Erro fatal: Falha ao alocar memória para a entrada da Hash!\n");
        exit(1);
    }
    
    novaEntry->pista = pista;
    novaEntry->suspeito = suspeito;
    
    // 3. Insere no início da lista (encadeamento)
    novaEntry->next = table->items[index];
    table->items[index] = novaEntry;
}

char* encontrarSuspeito(HashTable* table, char* pista) {
    // 1. Calcula o índice
    unsigned int index = hashFunction(pista, table->size);
    
    // 2. Percorre a lista encadeada (bucket)
    HashEntry* entry = table->items[index];
    while (entry != NULL) {
        if (strcmp(entry->pista, pista) == 0) {
            return entry->suspeito; // Encontrou
        }
        entry = entry->next;
    }
    
    return NULL; // Não encontrou
}

Sala* criarSala(char* nome, char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro fatal: Falha ao alocar memória para a sala '%s'!\n", nome);
        exit(1);
    }
    novaSala->nome = nome;
    novaSala->pista = pista;
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}


PistaNode* inserirPista(PistaNode* raiz, char* pista) {
    if (raiz == NULL) {
        PistaNode* novoNode = (PistaNode*)malloc(sizeof(PistaNode));
        if (novoNode == NULL) {
            printf("Erro fatal: Falha ao alocar memória para a pista!\n");
            exit(1);
        }
        novoNode->pista = pista;
        novoNode->esquerda = NULL;
        novoNode->direita = NULL;
        return novoNode;
    }

    int comparacao = strcmp(pista, raiz->pista);
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    return raiz;
}


void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}


void explorarSalas(Sala* salaInicial, PistaNode** raizPistas) {
    Sala* salaAtual = salaInicial;
    char escolha;

    while (salaAtual != NULL) {
        printf("\n===================================\n");
        printf("Você está em: %s\n", salaAtual->nome);
        printf("===================================\n");

        // 1. Lógica de Coleta de Pista
        if (salaAtual->pista != NULL) {
            printf("!! Você encontrou uma pista: %s\n", salaAtual->pista);
            *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
            salaAtual->pista = NULL; // Remove a pista da sala
        }

        // 2. Apresenta as opções
        printf("Escolha seu caminho:\n");
        if (salaAtual->esquerda != NULL) {
            printf(" (e) Ir para a Esquerda\n");
        }
        if (salaAtual->direita != NULL) {
            printf(" (d) Ir para a Direita\n");
        }
        printf(" (s) Sair do Jogo e fazer a acusação\n");
        printf("Sua escolha: ");

        // 3. Lê a escolha
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
                printf("\nSaindo da mansão para o julgamento...\n");
                return; // Volta para main()
            default:
                printf("\n-> Opção inválida! Tente 'e', 'd' ou 's'.\n");
                break;
        }
    }
}


int contarPistasParaSuspeito(PistaNode* raizPistas, HashTable* tabelaRespostas, char* suspeitoAcusado) {
    // Caso base da recursão
    if (raizPistas == NULL) {
        return 0;
    }
    
    int count = 0;
    
    // Verifica a pista atual (Raiz)
    // 1. Encontra o suspeito associado a esta pista na tabela de respostas
    char* suspeitoVerdadeiro = encontrarSuspeito(tabelaRespostas, raizPistas->pista);
    
    // 2. Se a pista existe na tabela E o suspeito bate com o acusado...
    if (suspeitoVerdadeiro != NULL && strcmp(suspeitoVerdadeiro, suspeitoAcusado) == 0) {
        count = 1; // ...conta +1
    }
    
    // 3. Soma com os resultados da esquerda e direita (Percurso completo)
    return count + 
           contarPistasParaSuspeito(raizPistas->esquerda, tabelaRespostas, suspeitoAcusado) + 
           contarPistasParaSuspeito(raizPistas->direita, tabelaRespostas, suspeitoAcusado);
}


void verificarSuspeitoFinal(PistaNode* raizPistas, HashTable* tabelaRespostas) {
    // 1. Lista as pistas coletadas
    printf("\n\n--- PISTAS COLETADAS (EM ORDEM ALFABÉTICA) ---\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(raizPistas);
    }
    printf("------------------------------------------------\n");

    // 2. Pede a acusação
    char acusacao[50];
    printf("Os suspeitos são: Mordomo, Jardineiro, Governanta\n");
    printf("Quem você acusa? ");
    
    // Lê a string da acusação (limitado a 49 caracteres para segurança)
    scanf("%49s", acusacao);

    // 3. Verifica as pistas
    int totalPistas = contarPistasParaSuspeito(raizPistas, tabelaRespostas, acusacao);

    // 4. Exibe o veredito
    printf("\n--- VEREDITO ---\n");
    printf("Você acusou: %s\n", acusacao);
    printf("Pistas encontradas contra esta pessoa: %d\n", totalPistas);

    if (totalPistas >= 2) {
        printf("\n*** ACUSAÇÃO SUSTENTADA! ***\n");
        printf("Você juntou provas suficientes e resolveu o mistério. Parabéns, detetive!\n");
    } else {
        printf("\n*** PISTAS INSUFICIENTES! ***\n");
        printf("Sua acusação é fraca. O verdadeiro culpado escapou...\n");
    }
}

void liberarBST(PistaNode* raiz) {
    if (raiz == NULL) return;
    liberarBST(raiz->esquerda);
    liberarBST(raiz->direita);
    free(raiz);
}

void liberarMapa(Sala* sala) {
    if (sala == NULL) return;
    liberarMapa(sala->esquerda);
    liberarMapa(sala->direita);
    free(sala);
}

void liberarHashTable(HashTable* table) {
    if (table == NULL) return;
    for (int i = 0; i < table->size; i++) {
        HashEntry* entry = table->items[i];
        while (entry != NULL) {
            HashEntry* temp = entry;
            entry = entry->next;
            free(temp); // Libera cada nó da lista encadeada
        }
    }
    free(table->items); // Libera o array de ponteiros
    free(table);        // Libera a estrutura da tabela
}


int main() {
    // 1. Inicializa a Árvore de Pistas (BST)
    PistaNode* raizPistas = NULL;

    // 2. Montagem do Mapa da Mansão (Árvore Binária)
    Sala* hall = criarSala("Hall de Entrada", NULL);
    hall->esquerda = criarSala("Sala de Estar", "Um diário antigo");
    hall->direita = criarSala("Biblioteca", "Carta com um selo estranho");
    hall->esquerda->esquerda = criarSala("Sala de Jantar", NULL);
    hall->esquerda->direita = criarSala("Jardim de Inverno", "Pétala de uma flor rara");
    hall->direita->esquerda = criarSala("Escritório", "Chave enferrujada");
    hall->direita->direita = criarSala("Quarto Principal", NULL);
    hall->esquerda->esquerda->esquerda = criarSala("Cozinha", "Frasco de veneno (vazio)");
    hall->direita->esquerda->direita = criarSala("Passagem Secreta", "Foto rasgada");

    // 3. Criação e Preenchimento da Tabela Hash (Respostas)
    HashTable* tabelaRespostas = criarHashTable(HASH_TABLE_SIZE);
    
    // Associando pistas aos suspeitos
    inserirNaHash(tabelaRespostas, "Um diário antigo", "Governanta");
    inserirNaHash(tabelaRespostas, "Carta com um selo estranho", "Mordomo");
    inserirNaHash(tabelaRespostas, "Pétala de uma flor rara", "Jardineiro");
    inserirNaHash(tabelaRespostas, "Chave enferrujada", "Governanta");
    inserirNaHash(tabelaRespostas, "Frasco de veneno (vazio)", "Jardineiro");
    inserirNaHash(tabelaRespostas, "Foto rasgada", "Mordomo");


    // 4. Início do Jogo (Exploração)
    printf("--- Bem-vindo ao Detective Quest: O Julgamento ---\n");
    printf("Explore a mansão, colete as pistas e descubra o culpado.\n");
    
    explorarSalas(hall, &raizPistas);

    // 5. Fase de Julgamento (Após o jogador sair)
    verificarSuspeitoFinal(raizPistas, tabelaRespostas);

    // 6. Limpeza de Memória
    liberarMapa(hall);
    liberarBST(raizPistas);
    liberarHashTable(tabelaRespostas);

    return 0;
}
