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