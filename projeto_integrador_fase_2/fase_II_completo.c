#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ================= ESTRUTURAS =================

typedef struct {
    int id;
    char nome[51];
    char categoria[31];
    float valor;
} Produto;

typedef struct No {
    Produto dado;
    struct No* proximo;
} No;

// ================= AUXILIARES =================

void limparQuebraLinha(char* str) {
    int len = strlen(str);

    while (len > 0 &&
          (str[len - 1] == '\n' ||
           str[len - 1] == '\r')) {

        str[len - 1] = '\0';
        len--;
    }
}

int contar_linhas(const char *nome_arquivo) {

    FILE *arquivo = fopen(nome_arquivo, "r");

    if (!arquivo)
        return -1;

    int linhas = 0;
    char buffer[256];

    if (fgets(buffer, sizeof(buffer), arquivo)) {

        while (fgets(buffer, sizeof(buffer), arquivo)) {
            linhas++;
        }
    }

    fclose(arquivo);
    return linhas;
}

// ================= LEITURA DO CSV =================

Produto* ler_produtos(
    const char *nome_arquivo,
    int *tamanho_final,
    int *linhas_corrompidas
) {

    *linhas_corrompidas = 0;

    int total_linhas = contar_linhas(nome_arquivo);

    if (total_linhas <= 0) {

        printf("Erro: arquivo inexistente ou vazio.\n");
        return NULL;
    }

    FILE *arquivo = fopen(nome_arquivo, "r");

    if (!arquivo) {

        printf("Erro ao abrir arquivo.\n");
        return NULL;
    }

    Produto *vetor =
        malloc(total_linhas * sizeof(Produto));

    if (!vetor) {

        printf("Erro de memoria.\n");
        fclose(arquivo);
        return NULL;
    }

    char linha[256];
    int k = 0;

    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo)) {

        limparQuebraLinha(linha);

        if (strlen(linha) == 0)
            continue;

        Produto p;

        int campos =
            sscanf(
                linha,
                "%d,%50[^,],%30[^,],%f",
                &p.id,
                p.nome,
                p.categoria,
                &p.valor
            );

        if (campos == 4) {

            vetor[k++] = p;

        } else {

            (*linhas_corrompidas)++;
        }
    }

    fclose(arquivo);

    *tamanho_final = k;

    return vetor;
}

// ================= HASH =================

int funcaoHash(int id, int tamanhoTabela)
{
    if (id < 0)
        id = -id;

    int x = (id / 1000) + (id % 1000);

    return x % tamanhoTabela;
}

void inserirHash(
    No** tabela,
    int tamanhoTabela,
    Produto p,
    int* contadorColisoes
) {

    int indice =
        funcaoHash(
            p.id,
            tamanhoTabela
        );

    No* novo =
        malloc(sizeof(No));

    if (!novo)
        return;

    novo->dado = p;
    novo->proximo = NULL;

    if (tabela[indice] != NULL) {

        (*contadorColisoes)++;

        novo->proximo =
            tabela[indice];
    }

    tabela[indice] = novo;
}

No* buscaHash(
    No** tabela,
    int tamanhoTabela,
    int idBuscado
) {

    int indice =
        funcaoHash(
            idBuscado,
            tamanhoTabela
        );

    No* atual =
        tabela[indice];

    while (atual != NULL) {

        if (atual->dado.id ==
            idBuscado) {

            return atual;
        }

        atual = atual->proximo;
    }

    return NULL;
}

void liberarTabelaHash(
    No** tabela,
    int tamanhoTabela
) {

    for (int i = 0;
         i < tamanhoTabela;
         i++) {

        No* atual = tabela[i];

        while (atual != NULL) {

            No* temp = atual;

            atual =
                atual->proximo;

            free(temp);
        }
    }

    free(tabela);
}

// ================= PROTOCOLO =================

void executarProtocoloExperimentalHash(
    No** tabelaHash,
    int tamanhoTabela,
    Produto* vetorOriginal
) {

    if (tamanhoTabela == 0)
        return;

    int testes[4];

    testes[0] =
        vetorOriginal[0].id;

    testes[1] =
        vetorOriginal[
            tamanhoTabela / 2
        ].id;

    testes[2] =
        vetorOriginal[
            tamanhoTabela - 1
        ].id;

    testes[3] = -88888;

    const char *nomes[4] = {
        "Inicio",
        "Meio",
        "Final",
        "Inexistente"
    };

    const int numBuscas = 1000;
    const int repeticoes = 3;

    volatile No* resultado;

    printf("\n");
    printf("==============================================================\n");
    printf("                 RESULTADOS - TABELA HASH\n");
    printf("==============================================================\n");
    printf("%-15s %-15s %-15s\n",
           "CASO",
           "TEMPO TOTAL",
           "TEMPO MEDIO");
    printf("--------------------------------------------------------------\n");

    for (int t = 0; t < 4; t++) {

        double somaTempoTotal = 0.0;

        for (int r = 0;
             r < repeticoes;
             r++) {

            clock_t inicio =
                clock();

            for (int i = 0;
                 i < numBuscas;
                 i++) {

                resultado =
                    buscaHash(
                        tabelaHash,
                        tamanhoTabela,
                        testes[t]
                    );
            }

            clock_t fim =
                clock();

            double tempo =
                ((double)
                (fim - inicio))
                / CLOCKS_PER_SEC;

            somaTempoTotal +=
                tempo;
        }

        double mediaTempoTotal =
            somaTempoTotal /
            repeticoes;

        double mediaBusca =
            mediaTempoTotal /
            numBuscas;

        printf(
            "%-15s %-15.6f %-15.10f\n",
            nomes[t],
            mediaTempoTotal,
            mediaBusca
        );
    }

    printf("==============================================================\n");
}

// ================= MAIN =================

int main() {

    const char *arquivo =
        "dataset2.csv";

    int totalRegistros = 0;
    int linhasCorrompidas = 0;

    Produto *produtos =
        ler_produtos(
            arquivo,
            &totalRegistros,
            &linhasCorrompidas
        );

    if (!produtos)
        return 1;

    printf("\n");
    printf("==============================================================\n");
    printf("CARACTERIZACAO DO DATASET\n");
    printf("==============================================================\n");
    printf("Arquivo: %s\n",
           arquivo);
    printf("Registros carregados: %d\n",
           totalRegistros);
    printf("Linhas corrompidas: %d\n",
           linhasCorrompidas);
    printf("==============================================================\n");

    int tamanhoTabela =
        totalRegistros;

    int totalColisoes = 0;

    No **tabelaHash =
        calloc(
            tamanhoTabela,
            sizeof(No*)
        );

    if (!tabelaHash) {

        free(produtos);
        return 1;
    }

    for (int i = 0;
         i < totalRegistros;
         i++) {

        inserirHash(
            tabelaHash,
            tamanhoTabela,
            produtos[i],
            &totalColisoes
        );
    }

    printf(
        "\nTotal de colisoes: %d\n",
        totalColisoes
    );

    executarProtocoloExperimentalHash(
        tabelaHash,
        tamanhoTabela,
        produtos
    );

    printf("\n");
    printf("==============================================================\n");
    printf("TABELA COMPARATIVA PARA O RELATORIO\n");
    printf("==============================================================\n");
    printf("%-25s %-20s %-20s\n",
           "METRICA",
           "SEQUENCIAL",
           "HASH");

    printf("--------------------------------------------------------------\n");

    printf("%-25s %-20s %-20s\n",
           "Complexidade Media",
           "O(n)",
           "O(1)");

    printf("%-25s %-20s %-20s\n",
           "Pior Caso",
           "O(n)",
           "O(n)");

    printf("%-25s %-20s %-20d\n",
           "Colisoes",
           "-",
           totalColisoes);

    printf("==============================================================\n");

    liberarTabelaHash(
        tabelaHash,
        tamanhoTabela
    );

    free(produtos);

    return 0;
}