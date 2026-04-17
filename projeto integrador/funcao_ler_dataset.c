#include <stdio.h>  
#include <stdlib.h>

// Conta quantas linhas existem no arquivo
int contar_linhas(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) return -1;

    int linhas = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), arquivo)) {
        linhas++;
    }

    fclose(arquivo);
    return linhas;
}

// Lê partes do dataset (início, meio e fim)
int* ler_partes_otimizado(const char *nome_arquivo, int *tamanho_final) {
    int total_linhas = contar_linhas(nome_arquivo);

    if (total_linhas < 1000) {
        printf("Dataset muito pequeno!\n");
        return NULL;
    }

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo\n");
        return NULL;
    }

    int inicio_qtd = 200;
    int meio_qtd = 500;
    int fim_qtd = 300;

    int total = inicio_qtd + meio_qtd + fim_qtd;
    int *ids = malloc(total * sizeof(int));

    if (ids == NULL) {
        printf("Erro de memória\n");
        fclose(arquivo);
        return NULL;
    }

    int meio_inicio = (total_linhas / 2) - (meio_qtd / 2);
    int fim_inicio = total_linhas - fim_qtd;

    char linha[256];
    int linha_atual = 0;
    int k = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        int id;

        if (sscanf(linha, "%d,", &id) == 1 || sscanf(linha, "%d", &id) == 1) {

            if (linha_atual < inicio_qtd) {
                ids[k++] = id;
            }
            else if (linha_atual >= meio_inicio && linha_atual < meio_inicio + meio_qtd) {
                ids[k++] = id;
            }
            else if (linha_atual >= fim_inicio) {
                ids[k++] = id;
            }

            linha_atual++;
        }
    }

    fclose(arquivo);

    *tamanho_final = k;
    return ids;
}