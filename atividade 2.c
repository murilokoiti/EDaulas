#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 2048


typedef struct Registro {
    char nome_completo[100];
    char cargo[100];
    char uorg_lotacao[100];
} Registro;


typedef struct Nodo {
    Registro dado;
    struct Nodo *esquerda, *direita;
    struct Nodo *proximo; 
} Nodo;


Nodo* criar_nodo(Registro registro) {
    Nodo *novo = (Nodo*)malloc(sizeof(Nodo));
    if (!novo) {
        fprintf(stderr, "Erro ao alocar memória!\n");
        exit(EXIT_FAILURE);
    }
    novo->dado = registro;
    novo->esquerda = novo->direita = NULL;
    novo->proximo = NULL; 
    return novo;
}


Nodo* inserir(Nodo *raiz, Registro registro) {
    if (raiz == NULL) 
        return criar_nodo(registro);

    if (strcasecmp(registro.nome_completo, raiz->dado.nome_completo) < 0)
        raiz->esquerda = inserir(raiz->esquerda, registro);
    else if (strcasecmp(registro.nome_completo, raiz->dado.nome_completo) > 0)
        raiz->direita = inserir(raiz->direita, registro);
    else {
        
        Nodo *novo = criar_nodo(registro);
        novo->proximo = raiz->proximo;
        raiz->proximo = novo;
    }

    return raiz;
}


Nodo* buscar(Nodo *raiz, const char *nome_completo) {
    if (raiz == NULL) return NULL;

    
    if (strcasecmp(raiz->dado.nome_completo, nome_completo) == 0)
        return raiz;

    if (strcasecmp(nome_completo, raiz->dado.nome_completo) < 0)
        return buscar(raiz->esquerda, nome_completo);
    else
        return buscar(raiz->direita, nome_completo);
}


Nodo* carregar_csv(const char *nome_arquivo, Nodo *raiz) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        fprintf(stderr, "Erro ao abrir o arquivo CSV!\n");
        exit(EXIT_FAILURE);
    }

    char linha[MAX_LINE];

    
    if (fgets(linha, MAX_LINE, arquivo) == NULL) {
        fprintf(stderr, "Erro: Arquivo CSV está vazio ou mal formatado!\n");
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }

    while (fgets(linha, MAX_LINE, arquivo)) {
        Registro registro = { "", "", "" };
        char *token;

        
        token = strtok(linha, ";");
        int coluna = 0;

        while (token != NULL) {
            
            if (token[0] == '"') token++;

            size_t len = strlen(token);
            if (token[len - 1] == '"') token[len - 1] = '\0';

            switch (coluna) {
                case 1: 
                    strncpy(registro.nome_completo, token, sizeof(registro.nome_completo) - 1);
                    break;
                case 4: 
                    strncpy(registro.cargo, token, sizeof(registro.cargo) - 1);
                    break;
                case 16: 
                    strncpy(registro.uorg_lotacao, token, sizeof(registro.uorg_lotacao) - 1);
                    break;
            }

            coluna++;
            token = strtok(NULL, ";");
        }

        
        if (strlen(registro.nome_completo) > 0 && strlen(registro.cargo) > 0) {
            raiz = inserir(raiz, registro);
        }
    }

    fclose(arquivo);
    return raiz;
}


void liberar_arvore(Nodo *raiz) {
    if (raiz == NULL) return;
    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);

    
    Nodo *atual = raiz;
    while (atual) {
        Nodo *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
}


int main() {
    Nodo *raiz = NULL;
    char nome_arquivo[] = "Unifesp-24r.csv";

    
    raiz = carregar_csv(nome_arquivo, raiz);

    
    char nome_completo[100];
    printf("Digite o nome completo para buscar: ");
    if (fgets(nome_completo, sizeof(nome_completo), stdin) == NULL) {
        fprintf(stderr, "Erro ao ler o nome!\n");
        liberar_arvore(raiz);
        exit(EXIT_FAILURE);
    }
    nome_completo[strcspn(nome_completo, "\n")] = '\0'; 

    
    Nodo *resultado = buscar(raiz, nome_completo);
    if (resultado) {
        int cont = 1;
        
        while (resultado) {
            printf("Registro %d:\n", cont);
            printf("Cargo: %s\n", resultado->dado.cargo);
            printf("UORG Lotacao: %s\n", resultado->dado.uorg_lotacao);
            cont++;
            resultado = resultado->proximo; 
        }
    } else {
        printf("Registro nao encontrado!\n");
    }

    
    liberar_arvore(raiz);
    return 0;
}
