int busca_sequencial_por_id(int *ids, int tamanho, int id_alvo) {
    for (int i = 0; i < tamanho; i++) {
        if (ids[i] == id_alvo) {
            return i;
        }
    }
    return -1;
}