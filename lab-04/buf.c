//
// Problema del búfer limitado.
//
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>

static void* producer(void*);
static void* consumer(void*);

struct buffer {
    int size;
    int* buf;
};

struct params {
    int wait_prod;
    int wait_cons;
    int items;
    struct buffer* buf;
} params_t;

/* Productor */
static void* producer(void *p)
{
    int i = 0;

    struct params *params = (struct params*) p;

    for (i = 0; i < params->items; i++) {
        params->buf->buf[i % params->buf->size] = i;
        // Espera una cantidad aleatoria de microsegundos.
        usleep(rand() % params->wait_prod);
    }

    pthread_exit(0);
}

/* Consumidor */
static void* consumer(void *p)
{
    int i;

    struct params *params = (struct params*) p;

    // Reserva memoria para guardar lo que lee el consumidor.
    int *reader_results = (int*) malloc(sizeof(int)*params->items);

    for (i = 0; i < params->items; i++) {
        reader_results[i] = params->buf->buf[i % params->buf->size];
        // Espera una cantidad aleatoria de microsegundos.
        printf("%d\n", rand() % params->wait_prod);
        usleep(rand() % params->wait_cons);
    }

    // Imprime lo que leyo
    for (i = 0; i < params->items; i++) {
        printf("%d ", reader_results[i]);
    }
    printf("\n");

    pthread_exit(0);
}

int main(int argc, char** argv)
{
    pthread_t producer_t, consumer_t;

    // Controla argumentos.
    if (argc != 5) {
        fprintf(stderr, "Uso: %s size items wait-prod wait-cons rand\n", argv[0]);
        fprintf(stderr, "\tsize:\ttamaño del buffer.\n");
        fprintf(stderr, "\titems:\tnúmero de items a producir/consumir.\n");
        fprintf(stderr, "\twaitp:\tnúmero de microsegundos que espera el productor.\n");
        fprintf(stderr, "\twaitc:\tnúmero de microsegundos que espera el consumidor.\n");
        exit(EXIT_FAILURE);
    }

    struct buffer *buf;
    buf = (struct buffer*) malloc(sizeof(struct buffer));
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Tamaño del buffer.
    buf->size = atoi(argv[1]);
    if (buf->size <= 0) {
        fprintf(stderr, "bufsize tiene que ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    // Crea el buffer
    buf->buf = (int*) malloc(sizeof(int) * buf->size);
    if (buf->buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Instancia una estructura de parámetros
    struct params *params;
    params = (struct params*) malloc(sizeof(struct params));
    if (params == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    params->buf = buf;

    // Cantidad de items a producir.
    params->items = atoi(argv[2]);
    if (params->items <= 0) {
        fprintf(stderr, "counter tiene que ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    params->wait_prod = atoi(argv[3]);
    if (params->wait_prod <= 0) {
        fprintf(stderr, "wait-prod tiene que ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    params->wait_cons = atoi(argv[4]);
    if (params->wait_cons <= 0) {
        fprintf(stderr, "cons-wait tiene que ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    // Inicializa semilla para números pseudo-aleatorios.
    srand(getpid());

    // Crea productor y consumidor
    pthread_create(&producer_t, NULL, producer, params);
    pthread_create(&consumer_t, NULL, consumer, params);

    // Mi trabajo ya esta hecho ...
    pthread_exit(NULL);
}
