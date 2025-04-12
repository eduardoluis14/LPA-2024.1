#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct lista_vizinhos_t{
    int id_vizinho;
    struct lista_vizinhos_t* prox;
}lista_vizinhos_t;

typedef struct no_t{
    int id;
    double pos_x;
    double pos_y;
    lista_vizinhos_t* lista_vizinhos;
}no_t;

typedef no_t* grafo_t;

bool lista_vizinhos_adicionar(int vizinho, lista_vizinhos_t **lista){
    lista_vizinhos_t *novo = (lista_vizinhos_t*) malloc(sizeof(lista_vizinhos_t));
    if(novo == NULL) return false;
    novo->id_vizinho = vizinho;
    novo->prox = *lista;
    *lista = novo;
    return true;
}

void lista_vizinhos_imprimir(lista_vizinhos_t *lista){
    lista_vizinhos_t* aux = lista;
    while(aux){
        printf("%d ",aux->id_vizinho);
        aux = aux->prox;
    }
}


grafo_t grafo_criar(int tam){
    grafo_t novo = (grafo_t) malloc(sizeof(no_t)* tam);
    return novo;
}

void grafo_atualizar_vizinhos(int tam, double raio_comunicacao, grafo_t grafo){
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){
            if(i != j){
                if(sqrt(pow(grafo[i].pos_x - grafo[j].pos_x, 2) + pow(grafo[i].pos_y - grafo[j].pos_y, 2)) < raio_comunicacao){
                    lista_vizinhos_adicionar(grafo[i].id, &grafo[j].lista_vizinhos);
                }
            }
        }
    }
}

void grafo_imprimir(int tam, grafo_t grafo){
    for(int i = 0; i < tam; i++){
        printf("NÓ %d: ", i);
        lista_vizinhos_imprimir(grafo[i].lista_vizinhos);
        printf("\n");
    }
}


int main(int argc, char **argv){
    if (argc < 2) { 
        fprintf(stderr, "Uso: %s <numero> <arquivo>\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", argv[1]);
        return 1;
    }

    int no;
    double raio;

    fscanf(file, "%d\t%lf\n", &no, &raio);
    grafo_t grafo = grafo_criar(no);        

    int i = 0;

    while(fscanf(file, "%d\t%lf\t%lf\n", &grafo[i].id, &grafo[i].pos_x, &grafo[i].pos_y) == 3){
        grafo[i].lista_vizinhos = NULL;
        i++;
    }
    grafo_atualizar_vizinhos(no,raio, grafo);
    grafo_imprimir(no,grafo);
}