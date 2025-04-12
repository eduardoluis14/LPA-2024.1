#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>

//declaração de funcções
int verificaSenha(char* senhaTeste, char* senhaCriptografada);
void gerarPalavras(char *pwd, int n);

// Variáveis globais
char *senhaCriptografada = "$6$7yCakIXevncmT6se$m002Lkf2BK6Qgyhc.c/PxMTvcmBAXYtIo"
 "kUKvWwvB5H5zCt5HhhPOlV8ygebOcSsgNqG74whVwN.8UF9WaGfs/";
int tamanhoMaximo = 3;
char caracteres[] = "abc";
int flag = 0; 

// Função recursiva para gerar todas as combinações de palavras
void gerarPalavras(char *pwd, int n) {
    if (n == 0 || flag == 1) {
        return;
    }

    for (int i = 0; i < strlen(caracteres); i++) {
        char novoPwd[100];
        snprintf(novoPwd, sizeof(novoPwd), "%s%c", pwd, caracteres[i]);
        if (!verificaSenha(novoPwd, senhaCriptografada)){
            printf("%s --> sim! Senha encontrada!\n", novoPwd);
            flag = 1;
            return;
        }else{
            //continue;
            printf("%s --> não\n", novoPwd);
        }
        gerarPalavras(novoPwd, n - 1);
        if(flag == 1){
            return;
        }
    }
}

/**
* @brief Verifica se uma senha em texto é a mesma de uma senha criptografada.
*
* @param senhaTeste Senha em texto que queremos ver se corresponde à senha criptografada.
* @param senhaCriptografada Senha criptografada como no arquivo /etc/shadow.
* @return int Retorna 0 (zero) se as senhas forem iguais. Outro valor, caso contrário.
*/

int verificaSenha(char* senhaTeste, char* senhaCriptografada) {
    char *senhaTesteCriptografada = crypt(senhaTeste, senhaCriptografada);
    return strcmp(senhaTesteCriptografada, senhaCriptografada);
}

int main() {
    for (int i = 1; i <= tamanhoMaximo; i++) {
        gerarPalavras("", i);
    }
    return 0;
}

