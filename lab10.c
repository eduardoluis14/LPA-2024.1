#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>


typedef struct {
    uint8_t daddr[6]; // Endereco MAC de destino
    uint8_t saddr[6]; // Endereco MAC de origem (source)
    uint16_t protocol; // Protocolo da próxima camada (IP!)
} ethernet_hdr_t;

typedef struct {
    uint8_t hdr_len:4, // Tamanho do Cabeçalho,
    version:4; // Versão do IP
    uint8_t tos; // Tipo de serviço
    uint16_t tot_len; // Tamanho total do IP
    uint16_t id; // Id do pacote
    uint16_t frag_off; // Fragmentado?
    uint8_t ttl; // Tempo de vida
    uint8_t protocol; // Protocolo próxima camada (TCP!)
    uint16_t check; // Checksum
    uint8_t saddr[4]; // Endereço IP de origem
    uint8_t daddr[4]; // Endereço IP de destino
} ip_hdr_t;

typedef struct {
    uint16_t sport; // Porta TCP de origem
    uint16_t dport; // Porta TCP de destino
    uint32_t seq; // Sequência
    uint32_t ack_seq; // Acknowledgement
    uint8_t reservado:4, // Não usado
    hdr_len:4; // Tamanho do cabecalho
    uint8_t flags; // Flags do TCP
    uint16_t window; // Tamanho da janela
    uint16_t check; // Checksum
    uint16_t urg_ptr; // Urgente
} tcp_hdr_t;

void print_mac_address(uint8_t *addr) {
    printf("%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
}

void print_ip_address(uint8_t *addr) {
    printf("%d.%d.%d.%d\n", addr[0], addr[1], addr[2], addr[3]);
}

int main(int argc, char *argv[]){
    FILE *file = fopen(argv[1], "rb");

    ethernet_hdr_t eth_hdr;
    fread(&eth_hdr, sizeof(ethernet_hdr_t), 1, file);
    printf("Lendo Ethernet ..\n");
    printf("  --> MAC de Origem: ");
    print_mac_address(eth_hdr.saddr);
    printf("  --> MAC de Destino: ");
    print_mac_address(eth_hdr.daddr);

    ip_hdr_t ip_hdr;
    fread(&ip_hdr, sizeof(ip_hdr_t), 1, file);
    printf("Lendo IP ..\n");
    printf("  --> Versão do IP: %d\n", ip_hdr.version);
    printf("  --> Tamanho do cabeçalho: %d bytes\n", ip_hdr.hdr_len * 4);
    printf("  --> Tamanho do pacote: %d bytes\n", ntohs(ip_hdr.tot_len));
    printf("  --> Endereço IP de Origem: ");
    print_ip_address(ip_hdr.saddr);
    printf("  --> Endereço IP de Destino: ");
    print_ip_address(ip_hdr.daddr);

    tcp_hdr_t tcp_hdr;
    fread(&tcp_hdr, sizeof(tcp_hdr_t), 1, file);
    printf("Lendo TCP ..\n");
    printf("  --> Porta de Origem: %d\n", ntohs(tcp_hdr.sport));
    printf("  --> Porta de Destino: %d\n", ntohs(tcp_hdr.dport));
    printf("  --> Tamanho do cabeçalho: %d bytes\n", tcp_hdr.hdr_len * 4);
    fseek(file,tcp_hdr.hdr_len*4 - sizeof(tcp_hdr_t), SEEK_CUR);

    int tam_dados = ntohs(ip_hdr.tot_len) - (ip_hdr.hdr_len * 4) - (tcp_hdr.hdr_len * 4);
    printf("Lendo Dados (HTTP) ..\n");
    printf("  --> Tamanho dos dados: %d bytes\n", tam_dados);
    printf("  --> Dados:\n");
    
    for(int i = 0; i < tam_dados; i++) {
        printf("%c", fgetc(file));
    }

    fclose(file);
    return 0;
}