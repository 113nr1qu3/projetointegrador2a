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