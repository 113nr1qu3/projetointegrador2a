// #include <stdio.h>
// #include <stdlib.h>

// // Declarações das funções (vindas das outras issues)
// int* ler_partes_otimizado(const char *nome_arquivo, int *tamanho_final);
// int busca_sequencial_por_id(int *ids, int tamanho, int id_alvo);

// int main() {
//     const char *nome_arquivo = "dataset2.csv";
//     int tamanho;

//     int *ids = ler_partes_otimizado(nome_arquivo, &tamanho);

//     if (ids == NULL) {
//         return 1;
//     }

//     printf("Dataset parcial carregado com %d IDs.\n", tamanho);

//     int id_alvo;
//     printf("Digite o ID a buscar: ");

//     if (scanf("%d", &id_alvo) != 1) {
//         printf("Entrada inválida!\n");
//         free(ids);
//         return 1;
//     }

//     int indice = busca_sequencial_por_id(ids, tamanho, id_alvo);

//     if (indice != -1) {
//         printf("ID %d encontrado na posicao %d\n", id_alvo, indice);
//     } else {
//         printf("ID nao encontrado\n");
//     }

//     free(ids);
//     return 0;
// }