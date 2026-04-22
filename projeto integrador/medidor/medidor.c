#include <stdio.h>
#include <time.h>
#include "medidor.h"

// Função 1: Marca o início do tempo
void iniciar_medicao(MedidorTempo *medidor) {
    medidor->inicio_atual = clock();
}

// Função 2: Marca o fim do tempo, calcula a média e imprime os resultados
void finalizar_medicao(MedidorTempo *medidor) {
    clock_t fim_atual = clock();
    
    // Calcula o tempo que a execução específica levou (em segundos)
    double tempo_execucao = ((double)(fim_atual - medidor->inicio_atual)) / CLOCKS_PER_SEC;
    
    // Atualiza o histórico
    medidor->tempo_acumulado += tempo_execucao;
    medidor->total_chamadas++;
    
    // Calcula a média geral
    double media_tempo = medidor->tempo_acumulado / medidor->total_chamadas;
    
    // Exibe os resultados
    printf("-> Tempo da execucao atual: %.6f segundos\n", tempo_execucao);
    printf("-> Tempo medio (%d execucoes): %.6f segundos\n", medidor->total_chamadas, media_tempo);
    printf("--------------------------------------------------\n");
}

void exibir_relatorio(MedidorTempo *medidor) {
    double media = medidor->tempo_acumulado / medidor->total_chamadas;
    printf("--------------------------------------------------\n");
    printf("Total de buscas: %d\n", medidor->total_chamadas);
    printf("Tempo acumulado: %.6f segundos\n", medidor->tempo_acumulado);
    printf("Tempo medio: %.6f segundos\n", media);
    printf("--------------------------------------------------\n");
}