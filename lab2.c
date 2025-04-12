#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct pessoa_t pessoa_t;
typedef struct lista_pessoas_t lista_pessoas_t;

typedef lista_pessoas_t** tabela_hash_t;

int tabela_hash_tam = 0; 
//variavel global

struct pessoa_t{
  char nome[50];
  long long int cpf;
  int idade;
};
struct lista_pessoas_t{
  pessoa_t* pessoa;
  struct lista_pessoas_t *prox;
};

bool lista_pessoas_adicionar(pessoa_t *pessoa, lista_pessoas_t **lista){
  lista_pessoas_t* nova = (lista_pessoas_t*) malloc(sizeof(lista_pessoas_t));
  if(nova == NULL) return false;
  nova->pessoa = pessoa;
  nova->prox = *lista;
  *lista = nova;
  return true;
  }

void lista_pessoas_listar(lista_pessoas_t *lista){
  while(lista){
    printf("- %s %lld %d\n", lista->pessoa->nome, lista->pessoa->cpf, lista->pessoa->idade);
    lista = lista->prox;
  }
}

tabela_hash_t tabela_hash_pessoas_criar(){
    tabela_hash_t tabela = (tabela_hash_t) malloc(sizeof(lista_pessoas_t*) * tabela_hash_tam);
    if(tabela){
      for(int i = 0; i < tabela_hash_tam; i++){
        tabela[i] = NULL;
      }
    }
  return tabela;
  }

int tabela_hash_pessoas_funcao(pessoa_t* pessoa){
  return pessoa->cpf % tabela_hash_tam;
}

bool tabela_hash_pessoas_adicionar(pessoa_t *pessoa, tabela_hash_t tabela_hash){
int posicao = tabela_hash_pessoas_funcao(pessoa);
  return lista_pessoas_adicionar(pessoa, &tabela_hash[posicao]);
  }

void tabela_hash_pessoas_listar(tabela_hash_t tabela_hash){
  for(int i = 0; i < tabela_hash_tam; i++){
    printf("POSIÇÃO %d DA TABELA HASH:\n", i);
    lista_pessoas_listar(tabela_hash[i]);
  }
}

int main(int argc, char **argv){
   if (argc < 3) { 
        fprintf(stderr, "Uso: %s <numero> <arquivo>\n", argv[0]);
        return 1;
    }

    if (sscanf(argv[1], "%d", &tabela_hash_tam) != 1) {
      printf("Erro: O argumento fornecido não é um número válido.\n");
      return 1;
  }

    FILE *file = fopen(argv[2], "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", argv[2]);
        return 1;
    }

    tabela_hash_t tabela = tabela_hash_pessoas_criar();
    pessoa_t p;

    while (fscanf(file, "%50[^\t]\t%lld\t%d\n", p.nome, &p.cpf, &p.idade) == 3) {
        pessoa_t *novaPessoa = (pessoa_t*) malloc(sizeof(pessoa_t));
        strcpy(novaPessoa->nome, p.nome);
        novaPessoa->cpf = p.cpf;
        novaPessoa->idade = p.idade;

        tabela_hash_pessoas_adicionar(novaPessoa, tabela);
    }

    tabela_hash_pessoas_listar(tabela);

    return 0;
}

/*
int main(){
  tabela_hash_tam =  1;
  tabela_hash_t tabela = tabela_hash_pessoas_criar();
  pessoa_t *p1 = (pessoa_t*) malloc(sizeof(pessoa_t));
  pessoa_t *p2 = (pessoa_t*) malloc(sizeof(pessoa_t));
  pessoa_t *p3 = (pessoa_t*) malloc(sizeof(pessoa_t));
  pessoa_t *p4 = (pessoa_t*) malloc(sizeof(pessoa_t));
  pessoa_t *p5 = (pessoa_t*) malloc(sizeof(pessoa_t));

  p1->idade=13;
  strcpy(p1->nome, "lius");
  p1->cpf = 93842987;

  tabela_hash_pessoas_adicionar(p1, tabela);

  tabela_hash_pessoas_listar(tabela);

}
*/