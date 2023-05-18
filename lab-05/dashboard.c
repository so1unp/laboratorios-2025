#include <stdio.h>
#include <stdlib.h>

#define HEIGHT  25  // Altura en caracteres de la pizarra
#define WIDTH   25  // Ancho en caracteres de la pizarra

struct dashboard {
    char dashboard[HEIGHT][WIDTH];
};

typedef struct dashboard dashboard_t;

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-w dashboard palabra x y: escribe palabra en dashboard en la posición (x,y).\n");
    fprintf(stderr, "\t-p dashboard: imprime el dashboard indicado.\n");
    fprintf(stderr, "\t-c dashboard: crea un dashboard con el nombre indicado.\n");
    fprintf(stderr, "\t-d dashboard: elimina el dashboard indicado.\n");
    fprintf(stderr, "\t-h imprime este mensaje.\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-') {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][1];

    switch(option) {
        case 'w':
            printf("Escribe %s en el dashboard %s en la posición (%d, %d).\n", argv[3], argv[2], atoi(argv[4]), atoi(argv[5]));
            break;
        case 'p':
            printf("Imprime dashboard.\n");
            break;
        case 'c':
            printf("Crea dashboard.\n");
            break;
        case 'd':
            printf("Borra dashboard.\n");
            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "Comando desconocido\n");
            exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
