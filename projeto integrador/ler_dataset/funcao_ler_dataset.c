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

#include <stdio.h>
#include <stdlib.h>

int* ler_partes_otimizado(const char *nome_arquivo, int *tamanho_final) {
    int total_linhas = contar_linhas(nome_arquivo);
    const int INICIO_QTD = 200, MEIO_QTD = 500, FIM_QTD = 300;
    const int TOTAL_ESPERADO = INICIO_QTD + MEIO_QTD + FIM_QTD;

    if (total_linhas < TOTAL_ESPERADO) {
        printf("Dataset insuficiente para a amostragem (Mínimo: %d linhas)\n", TOTAL_ESPERADO);
        *tamanho_final = 0;
        return NULL;
    }

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    int *ids = malloc(TOTAL_ESPERADO * sizeof(int));
    if (!ids) {
        fclose(arquivo);
        return NULL;
    }

    char linha[256];
    int k = 0;
    int linha_atual = 0;

    // 1. Pular Cabeçalho
    if (!fgets(linha, sizeof(linha), arquivo)) {
        fclose(arquivo);
        free(ids);
        return NULL;
    }

    // 2. Cálculo das zonas de interesse
    int meio_inicio = (total_linhas / 2) - (MEIO_QTD / 2);
    int fim_inicio = total_linhas - FIM_QTD;

    // 3. Loop de leitura com "pulos" lógicos
    while (fgets(linha, sizeof(linha), arquivo) && k < TOTAL_ESPERADO) {
        linha_atual++; // Começa em 1 (primeira linha após cabeçalho)

        // Verificamos se a linha atual está em uma das 3 zonas
        int na_zona = (linha_atual <= INICIO_QTD) || 
                      (linha_atual >= meio_inicio && linha_atual < meio_inicio + MEIO_QTD) ||
                      (linha_atual >= fim_inicio);

        if (na_zona) {
            int id;
            if (sscanf(linha, "%d", &id) == 1) {
                ids[k++] = id;
            }
        }
        
        // Otimização: Se já passamos do fim da zona do meio mas não chegamos na do fim,
        // poderíamos usar fseek para "pular", mas em CSVs de tamanho variável, 
        // o fgets continua sendo a forma mais segura de não perder a contagem de linhas.
    }

    fclose(arquivo);

    *tamanho_final = k;
    return ids;
}