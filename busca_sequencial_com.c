#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// ================= MEDIDOR =================
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

    double media = medidor->tempo_acumulado / medidor->total_chamadas;

    printf("-> Tempo atual: %.6f s\n", tempo);
    printf("-> Media (%d execucoes): %.6f s\n", medidor->total_chamadas, media);
    printf("----------------------------------------\n");
}

void exibir_relatorio(MedidorTempo *medidor) {
    if (medidor->total_chamadas == 0) return;

    double media = medidor->tempo_acumulado / medidor->total_chamadas;

    printf("\n======= RELATÓRIO =======\n");
    printf("Execucoes: %d\n", medidor->total_chamadas);
    printf("Tempo total: %.6f s\n", medidor->tempo_acumulado);
    printf("Tempo medio: %.6f s\n", media);
    printf("=========================\n");
}

// ================= BUSCA =================
int busca_sequencial_por_id(int *ids, int tamanho, int id_alvo) {
    for (int i = 0; i < tamanho; i++) {
        if (ids[i] == id_alvo) {
            return i;
        }
    }
    return -1;
}

// ================= ARQUIVO =================
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

//  VERSÃO INTELIGENTE 
int* ler_partes_otimizado(const char *nome_arquivo, int *tamanho_final) {
    int total_linhas = contar_linhas(nome_arquivo);

    if (total_linhas == -1) {
        printf("Erro: arquivo nao encontrado!\n");
        return NULL;
    }

    if (total_linhas == 0) {
        printf("Arquivo vazio!\n");
        return NULL;
    }

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir arquivo\n");
        return NULL;
    }

    //  Ajusta automaticamente os tamanhos
    int inicio_qtd = total_linhas * 0.2;
    int meio_qtd   = total_linhas * 0.5;
    int fim_qtd    = total_linhas * 0.3;

    int total = inicio_qtd + meio_qtd + fim_qtd;

    int *ids = malloc(total * sizeof(int));
    if (!ids) {
        printf("Erro de memoria\n");
        fclose(arquivo);
        return NULL;
    }

    int meio_inicio = (total_linhas / 2) - (meio_qtd / 2);
    int fim_inicio  = total_linhas - fim_qtd;

    char linha[256];
    int linha_atual = 0, k = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        int id;

        if (sscanf(linha, "%d,", &id) == 1 || sscanf(linha, "%d", &id) == 1) {

            if (linha_atual < inicio_qtd)
                ids[k++] = id;

            else if (linha_atual >= meio_inicio && linha_atual < meio_inicio + meio_qtd)
                ids[k++] = id;

            else if (linha_atual >= fim_inicio)
                ids[k++] = id;

            linha_atual++;
        }
    }

    fclose(arquivo);

    *tamanho_final = k;
    return ids;
}

// ================= MAIN =================
int main() {
    const char *arquivo = "dataset2.csv";
    int tamanho;

    MedidorTempo medidor = {0, 0.0, 0};

    int *ids = ler_partes_otimizado(arquivo, &tamanho);
    if (!ids) return 1;

    printf("Dataset carregado com %d IDs\n", tamanho);

    int id_alvo;
    printf("Digite o ID: ");
    if (scanf("%d", &id_alvo) != 1) {
        printf("Entrada inválida\n");
        free(ids);
        return 1;
    }

    iniciar_medicao(&medidor);

    int indice = busca_sequencial_por_id(ids, tamanho, id_alvo);

    finalizar_medicao(&medidor);

    if (indice != -1)
        printf("Encontrado na posicao %d\n", indice);
    else
        printf("Nao encontrado\n");

    exibir_relatorio(&medidor);

    free(ids);
    return 0;
}