#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printSequenceHeaderInfo(FILE *file) {
    unsigned char header_data[4];
    fread(header_data, 1, sizeof(header_data), file);

    unsigned int width = (header_data[0] << 4) + (header_data[1] >> 4);
    unsigned int height = ((header_data[1] & 0x0f) << 8) + header_data[2];
    unsigned int frame_rate = header_data[3] & 0x0f;

    const char *frame_rate_str;
    switch (frame_rate) {
        case 1:
            frame_rate_str = "23.976fps";
            break;
        case 2:
            frame_rate_str = "24.000fps";
            break;
        case 3:
            frame_rate_str = "25.000fps";
            break;
        case 4:
            frame_rate_str = "29.970fps";
            break;
        case 5:
            frame_rate_str = "30.000fps";
            break;
        case 6:
            frame_rate_str = "50.000fps";
            break;
        case 7:
            frame_rate_str = "59.940fps";
            break;
        default:
            frame_rate_str = "60.000fps";
            break;
    }

    printf("Width = %u, Height = %u -- Frame rate = %s\n", width, height, frame_rate_str);
}

void printPictureHeaderInfo(FILE *file) {
    unsigned char picture_data[2];
    fread(picture_data, 1, sizeof(picture_data), file);

    unsigned char picture_type = (picture_data[1] >> 3) & 0x07;

    const char *picture_type_str;
    switch (picture_type) {
        case 1:
            picture_type_str = "I";
            break;
        case 2:
            picture_type_str = "P";
            break;
        case 3:
            picture_type_str = "B";
            break;
        default:
            picture_type_str = "Desconhecido";
            break;
    }

    printf("Tipo = %s\n", picture_type_str);
}

int main(int argc, char *argv[]){
    FILE *file = fopen(argv[1], "rb");

    unsigned char bytes[4];

    while (fread(bytes, 1, sizeof(bytes), file) == sizeof(bytes)) {
        if (memcmp(bytes, "\x00\x00\x01", 3) != 0) {
            fseek(file, -3, SEEK_CUR);
            continue;
        }

        unsigned char stream_type = bytes[3];

        switch (stream_type) {
            case 0xB3:
                printf("--> Código: b3 -- Sequence Header -- ");
                printSequenceHeaderInfo(file);
                break;
            case 0xB8:
                printf("--> Código: b8 -- Group of Pictures\n");
                break;
            case 0x00:
                printf("--> Código: 00 -- Picture -- ");
                printPictureHeaderInfo(file);
                break;
            case 0x01:
                printf("--> Código: 01 -- Slice\n");
                break;
            default:
                printf("--> Código: %.2x -- Tipo de stream não implementado\n", stream_type);
                break;
        }
    }

    fclose(file);

    return 0;
}
