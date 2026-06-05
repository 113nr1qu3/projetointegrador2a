typedef struct {
    int id;
    char nome[51];
    char categoria[31];
    float valor;
} Produto;

typedef struct No {
    Produto dado;
    struct No* proximo;
} No;

typedef struct {
    int total_chamadas;
    double tempo_acumulado;
    clock_t inicio_atual;
} MedidorTempo;