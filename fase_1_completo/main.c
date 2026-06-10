#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// ================= PRODUTO =================

typedef struct {
    int id;
    char nome[51];
    char categoria[31];
    float valor;
} Produto;

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

    double tempo =
        ((double)(fim - medidor->inicio_atual)) / CLOCKS_PER_SEC;

    medidor->tempo_acumulado += tempo;
    medidor->total_chamadas++;
}

// ================= BUSCA =================

int busca_sequencial_por_id(
    Produto *produtos,
    int tamanho,
    int id_alvo
) {
    for (int i = 0; i < tamanho; i++) {
        if (produtos[i].id == id_alvo) {
            return i;
        }
    }

    return -1;
}

// ================= CSV =================

int contar_linhas(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");

    if (!arquivo)
        return -1;

    int linhas = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), arquivo)) {
        linhas++;
    }

    fclose(arquivo);
    return linhas;
}

Produto* ler_csv_completo(
    const char *nome_arquivo,
    int *tamanho
) {
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

    Produto *produtos =
        malloc(total_linhas * sizeof(Produto));

    if (!produtos) {
        printf("Erro de memoria\n");
        fclose(arquivo);
        return NULL;
    }

    char linha[256];
    int i = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {

        Produto p;

        if (sscanf(
                linha,
                "%d,%50[^,],%30[^,],%f",
                &p.id,
                p.nome,
                p.categoria,
                &p.valor
            ) == 4) {

            produtos[i++] = p;
        }
    }

    fclose(arquivo);

    *tamanho = i;

    return produtos;
}

// ================= MAIN =================

int main() {

    const char *arquivo = "dataset2.csv";

    int tamanho;

    Produto *produtos =
        ler_csv_completo(arquivo, &tamanho);

    if (!produtos)
        return 1;

    if (tamanho == 0) {
        printf("Nenhum registro valido encontrado.\n");
        free(produtos);
        return 1;
    }

    printf("\nQuantidade total de registros: %d\n", tamanho);

    int testes[4];

    testes[0] = produtos[0].id;
    testes[1] = produtos[tamanho / 2].id;
    testes[2] = produtos[tamanho - 1].id;
    testes[3] = -999999;

    const char *nomes_testes[4] = {
        "Inicio",
        "Meio",
        "Final",
        "Inexistente"
    };

    const int repeticoes = 1000;
    const int rodadas = 3;

    printf("\n===============================================================\n");
    printf("%-15s %-15s %-15s\n",
           "CASO",
           "TEMPO TOTAL",
           "TEMPO MEDIO");
    printf("===============================================================\n");

    volatile int resultado;

    for (int t = 0; t < 4; t++) {

        double soma_tempos_totais = 0.0;
        double soma_tempos_medios = 0.0;

        for (int r = 0; r < rodadas; r++) {

            MedidorTempo medidor = {0, 0.0, 0};

            for (int i = 0; i < repeticoes; i++) {

                iniciar_medicao(&medidor);

                resultado =
                    busca_sequencial_por_id(
                        produtos,
                        tamanho,
                        testes[t]
                    );

                finalizar_medicao(&medidor);
            }

            double tempo_total =
                medidor.tempo_acumulado;

            double tempo_medio =
                tempo_total /
                medidor.total_chamadas;

            soma_tempos_totais += tempo_total;
            soma_tempos_medios += tempo_medio;
        }

        double media_total =
            soma_tempos_totais / rodadas;

        double media_busca =
            soma_tempos_medios / rodadas;

        printf("%-15s %-15.6f %-15.10f\n",
               nomes_testes[t],
               media_total,
               media_busca);
    }

    printf("===============================================================\n");

    free(produtos);

    return 0;
}