#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// MEDIDOR
typedef struct {
    int total_chamadas;
    double tempo_acumulado;
    clock_t inicio_atual;
} MedidorTempo;

void iniciar_medicao(MedidorTempo *medidor) {
    medidor->inicio_atual = clock();
}

void finalizar_medicao(MedidorTempo *medidor) {
    clock_t fim = clock();

    double tempo = ((double)(fim - medidor->inicio_atual)) / CLOCKS_PER_SEC;

    medidor->tempo_acumulado += tempo;
    medidor->total_chamadas++;
}

void exibir_relatorio(MedidorTempo *medidor) {
    if (medidor->total_chamadas == 0) return;

    double media = medidor->tempo_acumulado / medidor->total_chamadas;

    printf("\n======= RELATÓRIO =======\n");
    printf("Execucoes: %d\n", medidor->total_chamadas);
    printf("Tempo total: %.6f s\n", medidor->tempo_acumulado);
    printf("Tempo medio: %.10f s\n", media);
    printf("=========================\n");
}

// BUSCA
int busca_sequencial_por_id(int *ids, int tamanho, int id_alvo) {
    for (int i = 0; i < tamanho; i++) {
        if (ids[i] == id_alvo) {
            return i;
        }
    }
    return -1;
}

//  ARQUIVO 
int contar_linhas(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) return -1;

    int linhas = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), arquivo)) {
        linhas++;
    }

    fclose(arquivo);
    return linhas;
}

// 🔹 Lê TODO o arquivo (CORRETO conforme professor pediu)
int* ler_csv_completo(const char *nome_arquivo, int *tamanho) {
    int total_linhas = contar_linhas(nome_arquivo);

    if (total_linhas <= 0) {
        printf("Erro: arquivo vazio ou inexistente\n");
        return NULL;
    }

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir arquivo\n");
        return NULL;
    }

    int *ids = malloc(total_linhas * sizeof(int));
    if (!ids) {
        printf("Erro de memoria\n");
        fclose(arquivo);
        return NULL;
    }

    char linha[256];
    int i = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        int id;

        if (sscanf(linha, "%d,", &id) == 1 || sscanf(linha, "%d", &id) == 1) {
            ids[i++] = id;
        }
    }

    fclose(arquivo);

    *tamanho = i;
    return ids;
}

// ================= MAIN =================
int main() {
    const char *arquivo = "dataset2.csv";
    int tamanho;

    int *ids = ler_csv_completo(arquivo, &tamanho);
    if (!ids) return 1;

    printf("Dataset carregado com %d IDs\n", tamanho);

    // 🔹 Casos de teste exigidos
    int testes[4];
    testes[0] = ids[0];                // início
    testes[1] = ids[tamanho / 2];      // meio
    testes[2] = ids[tamanho - 1];      // fim
    testes[3] = -999999;               // inexistente

    const char *nomes_testes[4] = {
        "Inicio do vetor",
        "Meio do vetor",
        "Final do vetor",
        "Elemento inexistente"
    };

    int repeticoes = 1000;

    // 🔹 Executa todos os teste
    for (int t = 0; t < 4; t++) {
        MedidorTempo medidor = {0, 0.0, 0};

        printf("\n===== TESTE: %s =====\n", nomes_testes[t]);

        for (int i = 0; i < repeticoes; i++) {
            iniciar_medicao(&medidor);
            busca_sequencial_por_id(ids, tamanho, testes[t]);
            finalizar_medicao(&medidor);
        }

        exibir_relatorio(&medidor);
    }

    free(ids);
    return 0;
}