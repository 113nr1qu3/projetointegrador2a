#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ================= ESTRUTURAS DE DADOS =================
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

typedef struct {
    int total_chamadas;
    double tempo_acumulado;
    clock_t inicio_atual;
} MedidorTempo;

// ================= FUNÇÕES DO MEDIDOR =================
void iniciar_medicao(MedidorTempo *medidor) {
    medidor->inicio_atual = clock();
}

void finalizar_medicao(MedidorTempo *medidor) {
    clock_t fim = clock();
    double tempo = ((double)(fim - medidor->inicio_atual)) / CLOCKS_PER_SEC;
    medidor->tempo_acumulado += tempo;
    medidor->total_chamadas++;
}

// ================= FUNÇÕES AUXILIARES E LEITURA COMPLETAMENTE TRATADA =================
void limparQuebraLinha(char* str) {
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

int contar_linhas(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) return -1;

    int linhas = 0;
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), arquivo)) { // Ignora cabeçalho
        while (fgets(buffer, sizeof(buffer), arquivo)) {
            linhas++;
        }
    }
    fclose(arquivo);
    return linhas;
}

// CORREÇÃO PROFESSOR: Tratamento rigoroso de erros na leitura e parsing do CSV
Produto* ler_produtos_otimizado(const char *nome_arquivo, int *tamanho_final, int *linhas_corrompidas) {
    int total_linhas = contar_linhas(nome_arquivo);
    *linhas_corrompidas = 0;

    if (total_linhas <= 0) {
        printf("[ERRO CRÍTICO]: Arquivo inexistente, corrompido ou vazio.\n");
        return NULL;
    }

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("[ERRO CRÍTICO]: Falha operacional ao abrir o arquivo para leitura.\n");
        return NULL;
    }

    int inicio_qtd = total_linhas * 0.2;
    int meio_qtd   = total_linhas * 0.5;
    int fim_qtd    = total_linhas * 0.3;
    int total_alocar = inicio_qtd + meio_qtd + fim_qtd;

    Produto *vetor = malloc(total_alocar * sizeof(Produto));
    if (!vetor) {
        printf("[ERRO CRÍTICO]: Falha de alocacao de memoria para o vetor de produtos.\n");
        fclose(arquivo);
        return NULL;
    }

    int meio_inicio = (total_linhas / 2) - (meio_qtd / 2);
    int fim_inicio  = total_linhas - fim_qtd;

    char linha[256];
    int linha_atual = 0, k = 0;

    if (!fgets(linha, sizeof(linha), arquivo)) {
        printf("[AVISO]: Falha ao ler o cabecalho do CSV.\n");
    }

    while (fgets(linha, sizeof(linha), arquivo)) {
        limparQuebraLinha(linha);
        if (strlen(linha) == 0) continue;

        Produto p;
        // Parsing rigoroso validando se todas as colunas esperadas foram lidas com sucesso
        int campos_lidos = sscanf(linha, "%d,%50[^,],%30[^,],%f", &p.id, p.nome, p.categoria, &p.valor);
        
        if (campos_lidos == 4) {
            if (linha_atual < inicio_qtd) {
                vetor[k++] = p;
            }
            else if (linha_atual >= meio_inicio && linha_atual < meio_inicio + meio_qtd) {
                vetor[k++] = p;
            }
            else if (linha_atual >= fim_inicio) {
                vetor[k++] = p;
            }
            linha_atual++;
        } else {
            // CORREÇÃO PROFESSOR: Contabiliza e reporta falhas de leitura específicas do CSV
            (*linhas_corrompidas)++;
        }
    }

    fclose(arquivo);
    *tamanho_final = k;
    return vetor;
}

// ================= LÓGICA DA TABELA HASH =================
int funcaoHash(int id, int tamanhoTabela) {
    if (id < 0) id = -id;
    return id % tamanhoTabela;
}

void inserirHash(No** tabela, int tamanhoTabela, Produto p, int* contadorColisoes) {
    int indice = funcaoHash(p.id, tamanhoTabela);
    
    No* novoNo = (No*) malloc(sizeof(No));
    if (!novoNo) return;
    novoNo->dado = p;
    novoNo->proximo = NULL;

    if (tabela[indice] != NULL) {
        (*contadorColisoes)++;
        novoNo->proximo = tabela[indice];
    }
    tabela[indice] = novoNo;
}

No* buscaHash(No** tabela, int tamanhoTabela, int idBuscado) {
    int indice = funcaoHash(idBuscado, tamanhoTabela);
    No* atual = tabela[indice];
    while (atual != NULL) {
        if (atual->dado.id == idBuscado) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void liberarTabelaHash(No** tabela, int tamanhoTabela) {
    for (int i = 0; i < tamanhoTabela; i++) {
        No* atual = tabela[i];
        while (atual != NULL) {
            No* temp = atual;
            atual = current->proximo; // alteração leve
            atual = atual->proximo;
            free(temp);
        }
    }
    free(tabela);
}

// ================= PROTOCOLO EXPERIMENTAL AUTOMATIZADO =================
void executarProtocoloExperimentalHash(No** tabelaHash, int tamanhoTabela, Produto* vetorOriginal) {
    if (tamanhoTabela == 0) return;

    int idInicio = vetorOriginal[0].id;
    int idMeio   = vetorOriginal[tamanhoTabela / 2].id;
    int idFim    = vetorOriginal[tamanhoTabela - 1].id;
    int idInexistente = -88888; 

    int numBuscas = 1000;    
    int numRepeticoes = 3;   
    
    int* IDsParaBuscar = (int*) malloc(numBuscas * sizeof(int));
    for (int i = 0; i < numBuscas; i++) {
        if (i % 4 == 0) IDsParaBuscar[i] = idInicio;
        else if (i % 4 == 1) IDsParaBuscar[i] = idMeio;
        else if (i % 4 == 2) IDsParaBuscar[i] = idFim;
        else IDsParaBuscar[i] = idInexistente;
    }

    MedidorTempo medidorGlobal = {0, 0.0, 0};

    printf("\n==================================================\n");
    printf("         RESULTADOS OBTIDOS - TABELA HASH         \n");
    printf("==================================================\n");
    printf("%-12s | %-18s | %-15s\n", "Repeticao", "Tempo Total (s)", "Tempo Medio por Busca (s)");
    printf("--------------------------------------------------\n");

    for (int r = 1; r <= numRepeticoes; r++) {
        MedidorTempo medidorBateria = {0, 0.0, 0};
        
        iniciar_medicao(&medidorBateria);
        for (int i = 0; i < numBuscas; i++) {
            buscaHash(tabelaHash, tamanhoTabela, IDsParaBuscar[i]);
        }
        finalizar_medicao(&medidorBateria);

        medidorGlobal.tempo_acumulado += medidorBateria.tempo_acumulado;
        medidorGlobal.total_chamadas++;

        double tempoMedioBusca = medidorBateria.tempo_acumulado / numBuscas;
        printf("Bateria %-5d | %-18.6f | %-15.8f\n", r, medidorBateria.tempo_acumulado, tempoMedioBusca);
    }
    printf("--------------------------------------------------\n");

    double mediaTempoTotal = medidorGlobal.tempo_acumulado / numRepeticoes;
    double mediaTempoBuscaFinal = mediaTempoTotal / numBuscas;

    // CORREÇÃO PROFESSOR: Saída rica em fundamentação técnica e caracterização
    printf("\n==================================================\n");
    printf("      SAIDA CONSOLIDADA (FASE II - HASH)          \n");
    printf("==================================================\n");
    printf("1. TAMANHO DA TABELA HASH (M): %d buckets.\n", tamanhoTabela);
    printf("2. ESCALA DO EXPERIMENTO: Executado lote de %d buscas automatizadas.\n", numBuscas);
    printf("3. TEMPO TOTAL MEDIO DO LOTE: %.6f segundos.\n", mediaTempoTotal);
    printf("4. TEMPO MEDIO POR BUSCA INDIVIDUAL: %.8f segundos.\n", mediaTempoBuscaFinal);
    printf("--------------------------------------------------\n");
    printf("FUNDAMENTAÇÃO TEÓRICA PARA O RELATÓRIO:\n");
    printf("- Complexidade Temporal Esperada da Hash: O(1) no caso médio.\n");
    printf("- Pior Caso Teórico: O(n), ocorre se todos os elementos colidirem no mesmo bucket.\n");
    printf("- Observação sobre a Busca Sequencial anterior:\n");
    printf("  O cenário 'Inexistente' representa o Pior Caso Matemático O(n)\n");
    printf("  porque obriga o algoritmo a varrer o array inteiro até a última\n");
    printf("  posição para certificar-se de que o elemento realmente não existe.\n");
    printf("==================================================\n");
    
    free(IDsParaBuscar);
}

// ================= MAIN =================
int main() {
    const char* nomeArquivo = "dataset2.csv"; 
    int totalRegistros = 0;
    int linhasCorrompidas = 0;

    // Carrega dados tratando erros de leitura
    Produto* vetorProdutos = ler_produtos_otimizado(nomeArquivo, &totalRegistros, &linhasCorrompidas);

    if (vetorProdutos != NULL) {
        int tamanhoTabelaHash = totalRegistros;
        int totalColisoes = 0;

        // CORREÇÃO PROFESSOR: Caracterização explícita do Dataset na inicialização
        printf("\n==================================================\n");
        printf("          CARACTERIZAÇÃO COMPLETA DO DATASET      \n");
        printf("==================================================\n");
        printf("-> Arquivo de origem: %s\n", nomeArquivo);
        printf("-> Volume total de registros validos carregados: %d\n", totalRegistros);
        printf("-> Linhas com falha/corrompidas no CSV ignoradas: %d\n", linhasCorrompidas);
        printf("==================================================\n");

        No** tabelaHash = (No**) calloc(tamanhoTabelaHash, sizeof(No*));
        if (tabelaHash == NULL) {
            free(vetorProdutos);
            return 1;
        }

        for (int i = 0; i < totalRegistros; i++) {
            inserirHash(tabelaHash, tamanhoTabelaHash, vetorProdutos[i], &totalColisoes);
        }

        printf(">>> Total de Colisoes registradas na Tabela Hash: %d <<<\n", totalColisoes);

        executarProtocoloExperimentalHash(tabelaHash, tamanhoTabelaHash, vetorProdutos);
        
        liberarTabelaHash(tabelaHash, tamanhoTabelaHash);
        free(vetorProdutos);
    }

    return 0;
}