#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct do produto
typedef struct {
    int codigo;
    char *nome;
    float preco;
    int quantidade;
} Produto;

// protótipos
void adicionar_produto(Produto **estoque, int *total, int *proximoCodigo);
void listar_produtos(Produto *estoque, int total);
Produto* buscar_produto(Produto *estoque, int total, int codigo);
void atualizar_estoque(Produto *estoque, int total, int codigo);
void remover_produto(Produto **estoque, int *total, int codigo);
void liberar_memoria(Produto **estoque, int *total);
void limpar_buffer();

int main() {
    Produto *estoque = NULL;
    int total = 0;
    int proximoCodigo = 1;
    int opcao, codigo;

    do {
     
        printf("\nSISTEMA DE CADASTRO DE PRODUTOS");
        printf("\nMenu:\n");
        printf("1. Adicionar produto\n");
        printf("2. Listar produtos\n");
        printf("3. Buscar produto\n");
        printf("4. Atualizar estoque\n");
        printf("5. Remover produto\n");
        printf("6. Sair\n");
        printf("\nOpcao: ");

        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            opcao = -1; // opcao invalida se o usuario digitar letra
        }

        switch (opcao) {
            case 1:
                printf("\n--- Adicionar Produto ---\n");
                adicionar_produto(&estoque, &total, &proximoCodigo);
                break;

            case 2:
                listar_produtos(estoque, total);
                break;

            case 3:
                printf("\n--- Buscar Produto ---\n");
                printf("Codigo do produto: ");
                scanf("%d", &codigo);

                Produto *p = buscar_produto(estoque, total, codigo);
                if (p != NULL) {
                    printf("\nNome: %s\n", p->nome);
                    printf("Preco: %.2f\n", p->preco);
                    printf("Quantidade: %d\n", p->quantidade);
                } else {
                    printf("Produto nao encontrado.\n");
                }
                break;

            case 4:
                printf("\nAtualizar Estoque\n");
                printf("Codigo do produto: ");
                scanf("%d", &codigo);
                atualizar_estoque(estoque, total, codigo);
                break;

            case 5:
                printf("\nRemover Produto\n");
                printf("Codigo do produto: ");
                scanf("%d", &codigo);
                remover_produto(&estoque, &total, codigo);
                break;

            case 6:
                liberar_memoria(&estoque, &total);
                printf("encerrado\n");
                break;

            default:
                printf("Opcao invalida\n");
        }

    } while (opcao != 6);

    return 0;
}

// tira o \n que fica sobrando no buffer depois do scanf de numero
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void adicionar_produto(Produto **estoque, int *total, int *proximoCodigo) {
    // aumenta o vetor em 1 posicao
    Produto *tmp = realloc(*estoque, (*total + 1) * sizeof(Produto));
    if (tmp == NULL) {
        printf("Nao foi possivel alocar memoria.\n");
        return;
    }
    *estoque = tmp;

    char nomeDigitado[100];

    limpar_buffer();
    printf("Nome: ");
    fgets(nomeDigitado, sizeof(nomeDigitado), stdin);
    nomeDigitado[strcspn(nomeDigitado, "\n")] = '\0'; // fgets pega o \n junto, entao tiro ele aqui

    // aloca so o espaco necessario pro nome (+1 do \0)
    (*estoque)[*total].nome = malloc(strlen(nomeDigitado) + 1);
    if ((*estoque)[*total].nome == NULL) {
        printf("Nao foi possivel alocar memoria pro nome.\n");
        return;
    }
    strcpy((*estoque)[*total].nome, nomeDigitado);

    printf("Preco: ");
    scanf("%f", &(*estoque)[*total].preco);

    printf("Quantidade: ");
    scanf("%d", &(*estoque)[*total].quantidade);

    (*estoque)[*total].codigo = *proximoCodigo;

    printf("Produto adicionado com codigo %d!\n", *proximoCodigo);

    (*total)++;
    (*proximoCodigo)++;
}

void listar_produtos(Produto *estoque, int total) {
    if (total == 0) {
        printf("\nEstoque vazio.\n");
        return;
    }

    float valorTotal = 0;

    printf("\n--- Lista de Produtos ---\n");
    printf("-------------------------------------------------------------------\n");
    printf("| Codigo | Nome                 | Preco    | Qtd  | Valor Estoque |\n");
    printf("-------------------------------------------------------------------\n");

    for (int i = 0; i < total; i++) {
        float valorItem = estoque[i].preco * estoque[i].quantidade;
        valorTotal += valorItem;

        printf("| %-6d | %-20s | %-8.2f | %-4d | %-13.2f |\n",
               estoque[i].codigo, estoque[i].nome, estoque[i].preco,
               estoque[i].quantidade, valorItem);
    }

    printf("-------------------------------------------------------------------\n");
    printf("Valor total do estoque: R$ %.2f\n", valorTotal);
}

// devolve o endereco do produto, ou NULL se nao achar
Produto* buscar_produto(Produto *estoque, int total, int codigo) {
    for (int i = 0; i < total; i++) {
        if (estoque[i].codigo == codigo) {
            return &estoque[i];
        }
    }
    return NULL;
}

void atualizar_estoque(Produto *estoque, int total, int codigo) {
    // usa a busca pra achar o produto, e mexe direto nele (passagem por referencia via ponteiro)
    Produto *p = buscar_produto(estoque, total, codigo);

    if (p == NULL) {
        printf("Produto nao encontrado");
        return;
    }

    printf("Quantidade atual: %d\n", p->quantidade);
    printf("Nova quantidade: ");
    scanf("%d", &p->quantidade);
    printf("Estoque atualizado\n");
}

void remover_produto(Produto **estoque, int *total, int codigo) {
    int indice = -1;

    for (int i = 0; i < *total; i++) {
        if ((*estoque)[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Produto nao encontrado");
        return;
    }

    printf("Produto \"%s\" removido", (*estoque)[indice].nome);

    // libera o nome ANTES de mexer no vetor, senao perde a referencia e vaza memoria
    free((*estoque)[indice].nome);

    // desloca tudo que vem depois uma posicao pra esquerda, cobrindo o buraco
    for (int i = indice; i < *total - 1; i++) {
        (*estoque)[i] = (*estoque)[i + 1];
    }

    (*total)--;

    if (*total == 0) {
        free(*estoque);
        *estoque = NULL;
    } else {
        Produto *tmp = realloc(*estoque, (*total) * sizeof(Produto));
        if (tmp != NULL) {
            *estoque = tmp;
        }
        // se realloc falhar aqui, o vetor continua do tamanho antigo mas funcional
    }
}

void liberar_memoria(Produto **estoque, int *total) {
    printf("\nLiberando memoria\n");

    // primeiro libera cada nome, que foi alocado separado
    for (int i = 0; i < *total; i++) {
        printf("Memoria do produto \"%s\" liberada\n", (*estoque)[i].nome);
        free((*estoque)[i].nome);
    }

    // so depois libera o vetor em si
    if (*estoque != NULL) {
        free(*estoque);
        *estoque = NULL;
    }

    *total = 0;
    printf("Vetor de produtos liberado\n");
}