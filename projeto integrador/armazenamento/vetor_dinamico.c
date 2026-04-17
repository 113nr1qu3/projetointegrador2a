#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDADE_INICIAL 100

// Estrutura do registro
typedef struct {
    char *campo1;
    char *campo2;
    char *campo3;
    char *campo4;
} Registro;

// Estrutura do vetor dinâmico
typedef struct {
    Registro *dados;
    int tamanho;
    int capacidade;
} Vetor;

// Inicializa o vetor
void inicializar_vetor(Vetor *v) {
    v->tamanho = 0;
    v->capacidade = CAPACIDADE_INICIAL;

    v->dados = malloc(v->capacidade * sizeof(Registro));
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
void adicionar_registro(Vetor *v,
                        const char *c1,
                        const char *c2,
                        const char *c3,
                        const char *c4) {

    if (v->tamanho >= v->capacidade) {
        v->capacidade *= 2;

        Registro *novo = realloc(v->dados, v->capacidade * sizeof(Registro));
        if (!novo) {
            perror("Erro ao expandir vetor");
            exit(EXIT_FAILURE);
        }

        v->dados = novo;
    }

    Registro *r = &v->dados[v->tamanho];

    r->campo1 = duplicar_string(c1);
    r->campo2 = duplicar_string(c2);
    r->campo3 = duplicar_string(c3);
    r->campo4 = duplicar_string(c4);

    if (!r->campo1 || !r->campo2 || !r->campo3 || !r->campo4) {
        perror("Erro ao alocar campos");
        exit(EXIT_FAILURE);
    }

    v->tamanho++;
}

// Acesso direto a um registro (útil para busca externa)
Registro *obter_registro(Vetor *v, int indice) {
    if (indice < 0 || indice >= v->tamanho)
        return NULL;

    return &v->dados[indice];
}

// Libera memória de um registro
void liberar_registro(Registro *r) {
    free(r->campo1);
    free(r->campo2);
    free(r->campo3);
    free(r->campo4);
}

// Libera vetor completo
void liberar_vetor(Vetor *v) {
    for (int i = 0; i < v->tamanho; i++) {
        liberar_registro(&v->dados[i]);
    }

    free(v->dados);
    v->dados = NULL;
    v->tamanho = 0;
    v->capacidade = 0;
}
