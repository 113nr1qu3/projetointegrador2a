#include <stdio.h>
#include <time.h>
#include "medidor.h"

void iniciar_medicao(MedidorTempo *medidor) {
    medidor->inicio_atual = clock();
}

void finalizar_medicao(MedidorTempo *medidor) {
    clock_t fim_atual = clock();
    double tempo_execucao = ((double)(fim_atual - medidor->inicio_atual)) / CLOCKS_PER_SEC;
    
    medidor->tempo_acumulado += tempo_execucao;
    medidor->total_chamadas++;
}

void exibir_relatorio_parcial(MedidorTempo *medidor) {
    double media = (medidor->total_chamadas > 0) ? (medidor->tempo_acumulado / medidor->total_chamadas) : 0;
    
    printf("\n--- Relatorio da Rodada %d ---\n", medidor->total_testes + 1);
    printf("Buscas realizadas: %d\n", medidor->total_chamadas);
    printf("Tempo da rodada:   %.6f s\n", medidor->tempo_acumulado);
    printf("Tempo medio/busca: %.6f s\n", media);
    printf("------------------------------\n");
}

void resetar_rodada(MedidorTempo *medidor) {
    medidor->tempo_total_geral += medidor->tempo_acumulado;
    medidor->total_testes++;
    
    // Zera para a próxima rodada
    medidor->tempo_acumulado = 0;
    medidor->total_chamadas = 0;
}