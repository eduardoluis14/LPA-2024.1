#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct evento_t{
    double tempo;
    int alvo;
    int tipo;
} evento_t;

struct lista_eventos_t{
    evento_t *evento;
    struct lista_eventos_t *proximo;
};

typedef struct lista_eventos_t lista_eventos_t;

typedef struct lista_vizinhos_t {
    int id;
    struct lista_vizinhos_t *prox;
} lista_vizinhos_t;

typedef struct no_t {
    int id;
    double pos_x;
    double pos_y;
    bool pacote_enviado;
    lista_vizinhos_t* lista_vizinhos;
} no_t;

typedef no_t* grafo_t;

bool lista_eventos_adicionar_ordenado(evento_t *evento, lista_eventos_t **lista);
bool lista_vizinhos_adicionar(int vizinho, lista_vizinhos_t **lista);
grafo_t grafo_criar(int tam); 
void grafo_atualizar_vizinhos(int tam, double raio_comunicacao, grafo_t grafo);
void simulacao_iniciar(lista_eventos_t **lista, grafo_t grafo);

bool lista_eventos_adicionar_ordenado(evento_t *evento, lista_eventos_t **lista){
  lista_eventos_t* item_novo = malloc(sizeof(lista_eventos_t));
  if(item_novo == NULL) return false;
  item_novo->evento = evento;
  if(*lista == NULL){
    *lista = item_novo;
    item_novo->proximo = NULL;
    return true;
  }
  lista_eventos_t* item_atual = *lista;
  if(item_novo->evento->tempo < item_atual->evento->tempo){
    item_novo->proximo = item_atual;
    *lista = item_novo;
    return true;
  }else{
    while(item_atual->proximo != NULL && item_atual->proximo->evento->tempo < item_novo->evento->tempo){
      item_atual = item_atual->proximo;
    }
    
  }
}

bool lista_vizinhos_adicionar(int vizinho, lista_vizinhos_t **lista){
    lista_vizinhos_t *novo = (lista_vizinhos_t*) malloc(sizeof(lista_vizinhos_t));
    if(novo == NULL) return false;
    novo->id = vizinho;
    novo->prox = *lista;
    *lista = novo;
    return true;
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

void simulacao_iniciar(lista_eventos_t **lista, grafo_t grafo){
  while (*lista != NULL) {

        lista_eventos_t *evento_atual = *lista;
        *lista = evento_atual->proximo;
        printf("[%3.6f] No %d recebeu pacote.\n", evento_atual->evento->tempo, evento_atual->evento->alvo);
        no_t *no_atual = &grafo[evento_atual->evento->alvo];

        if (!no_atual->pacote_enviado) {

            lista_vizinhos_t *vizinho_atual = no_atual->lista_vizinhos;
            while (vizinho_atual != NULL) {

                int id_vizinho = vizinho_atual->id;
                printf("\t--> Repassando pacote para o no %d ...\n", id_vizinho);
                evento_t *novo_evento = (evento_t*)malloc(sizeof(evento_t));
                novo_evento->tempo = evento_atual->evento->tempo + (0.1 + (id_vizinho * 0.01));
                novo_evento->alvo = id_vizinho;
                novo_evento->tipo = 1;
                lista_eventos_adicionar_ordenado(novo_evento, lista);
                vizinho_atual = vizinho_atual->prox;

            }

            no_atual->pacote_enviado = true;
        }

        free(evento_atual->evento);
        free(evento_atual);
  }
}

int main(int argc, char **argv){
  if (argc < 1) { 
        fprintf(stderr, "Uso: %s <numero> <arquivo>\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", argv[1]);
        return 1;
    }

    int tam;
    double raio_comunicacao;
    fscanf(file, "%d%lf\n", &tam, &raio_comunicacao);

    grafo_t grafo = grafo_criar(tam);

    for (int i = 0; i < tam; i++){
      fscanf(file,"%d%lf%lf\n",&grafo[i].id, &grafo[i].pos_x, &grafo[i].pos_y);
      grafo[i].lista_vizinhos = NULL;
      grafo[i].pacote_enviado = false;
	}

  grafo_atualizar_vizinhos(tam, raio_comunicacao, grafo);
	lista_eventos_t *lista = NULL;
  evento_t *evento = malloc(sizeof(evento_t));
	evento->tempo = 1.0;
	evento->alvo = 0;
	evento->tipo = 1;

	lista_eventos_adicionar_ordenado(evento,&lista);
	simulacao_iniciar(&lista,grafo);
}