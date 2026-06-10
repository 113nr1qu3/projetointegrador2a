#ifndef MEDIDOR_H
#define MEDIDOR_H

#include <time.h>

typedef struct {
    int total_chamadas;
    double tempo_acumulado;
    clock_t inicio_atual;
} MedidorTempo;

void iniciar_medicao(MedidorTempo *medidor);
void finalizar_medicao(MedidorTempo *medidor);
void exibir_relatorio(MedidorTempo *medidor);

#endif