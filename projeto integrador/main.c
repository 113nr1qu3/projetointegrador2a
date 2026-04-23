#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h> // Necessário para SetConsoleOutputCP
#endif
#include "medidor/medidor.h"
#include "armazenamento/vetor_armazenamento.h"


// Declarações das funções (vindas das outras issues)
int* ler_partes_otimizado(const char *nome_arquivo, int *tamanho_final);
int busca_sequencial_por_id(Vetor *vetor, int id_alvo);


int main() {
    // --- CONFIGURAÇÃO UTF-8 ---
    setlocale(LC_ALL, ".65001");
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    // ---------------------------
    // 1. Declarar e inicializar o medidor
    MedidorTempo meuMedidor = {0, 0.0, 0, };
    const char *nome_arquivo = "dataset2.csv";
    int tamanho;


    Vetor *vetor = ler_csv_final(nome_arquivo);    
    Produto *dataset = vetor->dados;
    int dataset_tamanho = vetor->tamanho;
    
    int *alvos = ler_partes_otimizado(nome_arquivo, &tamanho);
    
    if (alvos == NULL || vetor == NULL) {
        return 1;
    }


    //repetição de teste por 5 vezes
    for (int teste = 0; teste < 5; teste++) {

    for (int i = 0; i < tamanho; i++) {
        iniciar_medicao(&meuMedidor);

        int indice = busca_sequencial_por_id(vetor, alvos[i]);

        finalizar_medicao(&meuMedidor);

    }
    
    exibir_relatorio_parcial(&meuMedidor);
    resetar_rodada(&meuMedidor); // Prepara para a próxima das 5 rodadas
}

    // Relatório Final Consolidado
    double media_final = meuMedidor.tempo_total_geral / meuMedidor.total_testes;

    printf("\n=== RESULTADO FINAL CONSOLIDADO ===\n");
    printf("Total de rodadas de teste: %d\n", meuMedidor.total_testes);
    printf("Tempo total acumulado:     %.6f segundos\n", meuMedidor.tempo_total_geral);
    printf("Tempo médio por rodada:    %.6f segundos\n", media_final);
    printf("===================================\n");

    return 0;
}