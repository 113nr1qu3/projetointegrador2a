#include "../armazenamento/vetor_armazenamento.h"

int busca_sequencial_por_id(Vetor *vetor, int id_alvo) {
    for (int i = 0; i < vetor->tamanho; i++) {
        if (vetor->dados[i].id == id_alvo) {
            return i;
        }
    }
    return -1;
}