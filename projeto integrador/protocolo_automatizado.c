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

        No** tabelaHash = (No*) calloc(tamanhoTabelaHash, sizeof(No));
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