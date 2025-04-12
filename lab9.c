#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    const char *arq_entrada = argv[1];
    long tam = atol(argv[2]);

    FILE *entrada, *saida;
    char nomeSaida[50];
    char bytes[4];
    char *buffer;
    long maxTam = tam * 1024 * 1024;
    long tamCur = 0;
    int parte = 2;
    int idBuffer = 0;

    entrada = fopen(arq_entrada, "rb");

    buffer = (char *)malloc(maxTam);

    saida = fopen("video_parte_1.mpg", "wb");
    printf("Criando arquivo video_parte_1.mpg ..\n");

    while (1) {
        fread(bytes, sizeof(bytes), 1, entrada);
        if (memcmp(bytes, "\x00\x00\x01\xB3", 4) == 0 || feof(entrada)) {
            if (tamCur + idBuffer > maxTam) {
                fclose(saida);

                sprintf(nomeSaida, "video_parte_%d.mpg", parte++);
                printf("Criando arquivo %s ..\n", nomeSaida);
                
                saida = fopen(nomeSaida, "wb");
                fwrite(buffer, 1, idBuffer, saida);
                tamCur = idBuffer;
            } else {
                fwrite(buffer, 1, idBuffer, saida);
                tamCur += idBuffer;
            }
            memcpy(buffer, bytes, 4);
            idBuffer = 4;
        } else {
            buffer[idBuffer] = bytes[0];
            idBuffer++;
            fseek(entrada, -3, SEEK_CUR);
        }
        if (feof(entrada)){
            break;
        }
    }

    fclose(entrada);
    fclose(saida);
    free(buffer);

    return 0;
}