#ifndef LER_CSV_MAIN_H
#define LER_CSV_MAIN_H

#include <stdio.h>

// 1. Definição das Estruturas (O Main precisa saber o que é um Registro e um Vetor)
typedef struct {
int id;
char nome[51];
char categoria[31];
float valor;
} Produto;

typedef struct {
    Produto *dados;
    int tamanho;
    int capacidade;
} Vetor;

// 2. Protótipos das Funções (O "Menu" de opções que o Main pode chamar)
Vetor* ler_csv_final(const char *nome_arquivo);
void inicializar_vetor(Vetor *v);
void adicionar_produto(Vetor *v, int id, const char *nome, const char *categoria, float valor);
void liberar_vetor(Vetor *v);

#endif