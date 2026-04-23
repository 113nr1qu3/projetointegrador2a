#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vetor_armazenamento.h"

#define CAPACIDADE_INICIAL 100


// Inicializa o vetor
void inicializar_vetor(Vetor *v) {
    v->tamanho = 0;
    v->capacidade = CAPACIDADE_INICIAL;

    v->dados = malloc(v->capacidade * sizeof(Produto));
    if (!v->dados) {
        perror("Erro ao alocar vetor");
        exit(EXIT_FAILURE);
    }
}

// Duplica string
char *duplicar_string(const char *origem) {
    char *copia = malloc(strlen(origem) + 1);
    if (!copia) return NULL;

    strcpy(copia, origem);
    return copia;
}

// Adiciona registro
void adicionar_registro(Vetor *v, const char *c1, const char *c2, const char *c3, const char *c4) {
    if (v->tamanho >= v->capacidade) {
        v->capacidade *= 2;
        Produto *novo = realloc(v->dados, v->capacidade * sizeof(Produto));
        if (!novo) exit(EXIT_FAILURE);
        v->dados = novo;
    }

    Produto *p = &v->dados[v->tamanho];

    // Converte e copia
    p->id = atoi(c1);
    
    // Copia com segurança para não estourar os 51 e 31 bytes
    strncpy(p->nome, c2, 50);
    p->nome[50] = '\0'; // Garante o fim da string
    
    strncpy(p->categoria, c3, 30);
    p->categoria[30] = '\0';

    p->valor = atof(c4);

    v->tamanho++;
}

// Acesso direto a um registro (útil para busca externa)
Produto *obter_produto(Vetor *v, int indice) {
    if (indice < 0 || indice >= v->tamanho)
        return NULL;

    return &v->dados[indice];
}

Vetor* ler_csv_final(const char *nome_arquivo) {
    // Agora v é um ponteiro alocado no Heap
    Vetor *v = malloc(sizeof(Vetor));
    if (!v) {
        perror("Erro ao alocar estrutura Vetor");
        exit(EXIT_FAILURE);
    }
    
    inicializar_vetor(v); 

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo: %s\n", nome_arquivo);
        return v; 
    }

    char linha[1024];
    fgets(linha, sizeof(linha), arquivo); // Pula cabeçalho

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0;

        char *c1 = strtok(linha, ",");
        char *c2 = strtok(NULL, ",");
        char *c3 = strtok(NULL, ",");
        char *c4 = strtok(NULL, ",");

        if (c1 && c2 && c3 && c4) {
            // REMOVIDO o '&', pois 'v' já é um ponteiro!
            adicionar_registro(v, c1, c2, c3, c4);
        }
    }

    fclose(arquivo);
    
    return v;
}



// Libera memória de um produto
void liberar_produto(Produto *p) {
    // Nada a liberar para campos simples
}

// Libera vetor completo
void liberar_vetor(Vetor *v) {
    for (int i = 0; i < v->tamanho; i++) {
        liberar_produto(&v->dados[i]);
    }

    free(v->dados);
    v->dados = NULL;
    v->tamanho = 0;
    v->capacidade = 0;
}
