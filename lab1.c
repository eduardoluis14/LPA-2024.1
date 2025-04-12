#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Primeiramente são definidas as estruturas
typedef struct evento_t evento_t;
typedef struct lista_eventos_t lista_eventos_t;

struct evento_t{
  double tempo;
  int alvo;
  int tipo;
};

struct lista_eventos_t{
  evento_t* evento;
  struct lista_eventos_t* prox;
};

//Aqui são definidas as funções
//questao1

bool lista_eventos_adicionar_inicio(evento_t *evento, lista_eventos_t **lista){
  lista_eventos_t* novo = malloc(sizeof(lista_eventos_t));
  if(novo == NULL) return false;
  novo->evento = evento;
  novo->prox = *lista;
  *lista = novo;
  return true;
  }

void lista_eventos_listar(lista_eventos_t *lista){
  while(lista != NULL){
    printf("tempo: %f, alvo: %d, tipo: %d\n", lista->evento->tempo, lista->evento->alvo, lista->evento->tipo);
    lista = lista->prox;
  }
}

//questao 2
bool lista_eventos_adicionar_fim(evento_t *evento, lista_eventos_t **lista){
  lista_eventos_t* item_novo = malloc(sizeof(lista_eventos_t));
  if(item_novo == NULL) return false;
  item_novo->evento = evento;
  item_novo->prox = NULL;
  if(*lista == NULL){
    *lista = item_novo;
  }else{
    lista_eventos_t* item_atual = *lista;
    while(item_atual->prox != NULL){
      item_atual = item_atual->prox;
    }
    item_atual->prox = item_novo;
  }
  return true;
}

//questao 3
bool lista_eventos_adicionar_ordenado(evento_t *evento, lista_eventos_t **lista){
  lista_eventos_t* item_novo = malloc(sizeof(lista_eventos_t));
  if(item_novo == NULL) return false;
  item_novo->evento = evento;
  if(*lista == NULL){
    *lista = item_novo;
    item_novo->prox = NULL;
    return true;
  }
  lista_eventos_t* item_atual = *lista;
  if(item_novo->evento->tempo < item_atual->evento->tempo){
    item_novo->prox = item_atual;
    *lista = item_novo;
    return true;
  }else{
    while(item_atual->prox != NULL && item_atual->prox->evento->tempo < item_novo->evento->tempo){
      item_atual = item_atual->prox;
    }
    item_novo->prox = item_atual->prox;
    item_atual->prox = item_novo;
  }
  return true;
}

//aqui é a função main para testar o código
int main(int argc, char *argv[]){
  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
      printf("erro ao abrir o arquivo.\n");
      return 1;
  }
  lista_eventos_t *lista = NULL;
  double n1;
  int n2, n3;

  while (fscanf(file, "%lf\t%d\t%d\n", &n1, &n2, &n3) == 3) {
    evento_t* novoEvento = malloc(sizeof(evento_t*));
    if (novoEvento == NULL) {
        printf("erro ao alocar memória.\n");
        return 1;
    }

    novoEvento->tempo = n1;
    novoEvento->alvo = n2;
    novoEvento->tipo = n3;

    lista_eventos_adicionar_ordenado(novoEvento, &lista);
  }
  fclose(file);
  lista_eventos_listar(lista);
}
