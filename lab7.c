#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

struct png_chunk_hdr {
    int length;
    char type[4];
}__attribute__((packed));

struct png_chunk_IHDR {
    int width;
    int height;
    char bit_depth;
    char colour_type;
    char compression_method;
    char filter_method;
    char interlace_method;
}__attribute__((packed));

int main(int argc, char *argv[]) {

    FILE *png_file = fopen(argv[1], "rb");
    if (!png_file) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    struct png_chunk_hdr* chunk_hdr = malloc(sizeof(struct png_chunk_hdr));
    struct png_chunk_IHDR* ihdr_data = malloc(sizeof(struct png_chunk_IHDR));

    fseek(png_file, 8, SEEK_SET);

    int chunk_count = 0;

    while (!feof(png_file)) {
        // Ler o cabeçalho do chunk
        fread(chunk_hdr, sizeof(struct png_chunk_hdr), 1, png_file);
        
        printf("Lendo chunk %d:\n", chunk_count++);
        printf("    --> Tamanho: %d\n", ntohl(chunk_hdr->length));
        printf("    --> Tipo:    %.4s\n", chunk_hdr->type);

        if (strcmp(chunk_hdr->type, "IHDR") == 0) {
            fread(ihdr_data, sizeof(struct png_chunk_IHDR), 1, png_file);

            printf("        --> Largura: %d\n", ntohl(ihdr_data->width));
            printf("        --> Altura:  %d\n", ntohl(ihdr_data->height));

            fseek(png_file, 4, SEEK_CUR); 
        } else if (strcmp(chunk_hdr->type, "IEND") == 0) {
            break;
        } else {
            fseek(png_file, ntohl(chunk_hdr->length) + 4, SEEK_CUR);
        }
    }
    free(chunk_hdr);
    free(ihdr_data);
    fclose(png_file);
    
    return 0;
}
