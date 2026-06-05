void iniciar_medicao(MedidorTempo *medidor) {
    medidor->inicio_atual = clock();
}

void finalizar_medicao(MedidorTempo *medidor) {
    clock_t fim = clock();
    double tempo = ((double)(fim - medidor->inicio_atual)) / CLOCKS_PER_SEC;
    medidor->tempo_acumulado += tempo;
    medidor->total_chamadas++;
}