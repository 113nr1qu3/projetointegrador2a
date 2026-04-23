#ifndef MEDIDOR_H
#define MEDIDOR_H

#include <time.h>

typedef struct {
    int total_chamadas;      // Total de buscas na rodada atual
    double tempo_acumulado;  // Soma dos tempos da rodada atual
    double tempo_total_geral; // Soma de todas as rodadas
    int total_testes;        // Contador de rodadas (testes)
    clock_t inicio_atual;
} MedidorTempo;

void iniciar_medicao(MedidorTempo *medidor);
void finalizar_medicao(MedidorTempo *medidor);
void exibir_relatorio_parcial(MedidorTempo *medidor);
void resetar_rodada(MedidorTempo *medidor);

#endif