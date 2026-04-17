#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAMPOS_ESPERADOS 4

// Lê uma linha de tamanho variável
char *ler_linha(FILE *arquivo) {
    size_t tamanho = 1024;
    size_t comprimento = 0;
    char *buffer = malloc(tamanho);

    if (!buffer) return NULL;

    while (fgets(buffer + comprimento, tamanho - comprimento, arquivo)) {
        comprimento += strlen(buffer + comprimento);

        if (comprimento > 0 && buffer[comprimento - 1] == '\n')
            break;

        tamanho *= 2;
        char *novo_buffer = realloc(buffer, tamanho);
        if (!novo_buffer) {
            free(buffer);
            return NULL;
        }
        buffer = novo_buffer;
    }

    if (comprimento == 0) {
        free(buffer);
        return NULL;
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';
    return buffer;
}

// Parser CSV simples 
int parsear_csv_simples(const char *linha, char ***campos_saida) {
    int quantidade = 0;
    int capacidade = 10;

    char **campos = malloc(capacidade * sizeof(char *));
    if (!campos) return -1;

    const char *inicio = linha;

    while (*inicio) {
        if (quantidade >= capacidade) {
            capacidade *= 2;
            char **tmp = realloc(campos, capacidade * sizeof(char *));
            if (!tmp) return -1;
            campos = tmp;
        }

        const char *fim = inicio;
        while (*fim && *fim != ',') {
            fim++;
        }

        int tamanho = fim - inicio;

        char *campo = malloc(tamanho + 1);
        if (!campo) return -1;

        strncpy(campo, inicio, tamanho);
        campo[tamanho] = '\0';

        campos[quantidade++] = campo;

        if (*fim == ',')
            inicio = fim + 1;
        else
            break;
    }

    *campos_saida = campos;
    return quantidade;
}

// Libera memória
void liberar_campos(char **campos, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        free(campos[i]);
    }
    free(campos);
}

int main() {
    FILE *arquivo = fopen("dados.csv", "r");

    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return EXIT_FAILURE;
    }

    int numero_linha = 0;
    char *linha;

    while ((linha = ler_linha(arquivo)) != NULL) {
        numero_linha++;

        if (strlen(linha) == 0) {
            free(linha);
            continue;
        }

        char **campos = NULL;
        int quantidade_campos = parsear_csv_simples(linha, &campos);

        if (quantidade_campos < 0) {
            printf("Erro ao processar linha %d\n", numero_linha);
            free(linha);
            continue;
        }

        // Validação: exatamente 4 colunas
        if (quantidade_campos != CAMPOS_ESPERADOS) {
            printf("Erro na linha %d: esperado %d campos, encontrado %d\n",
                   numero_linha, CAMPOS_ESPERADOS, quantidade_campos);
            liberar_campos(campos, quantidade_campos);
            free(linha);
            continue;
        }

        // Processamento (exemplo)
        printf("Linha %d:\n", numero_linha);
        for (int i = 0; i < quantidade_campos; i++) {
            printf("  Campo %d: %s\n", i + 1, campos[i]);
        }

        liberar_campos(campos, quantidade_campos);
        free(linha);
    }

    fclose(arquivo);
    return EXIT_SUCCESS;
}