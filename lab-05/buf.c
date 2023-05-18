// 
// Problema del búfer limitado modificado.
// 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>

// Mueve el cursor a la linea x, columna y (ver codigos ANSI).
#define xy(x, y) printf("\033[%d;%dH", x, y)

// Borra desde el cursor hasta el fin de la linea.
#define clear_eol(x) print(x, 10, "\033[K")

// Borra la pantalla.
#define clear() printf("\033[H\033[J")

static void* producer(void*);
static void* consumer(void*);
static void setup_draw(void);
static void draw(int, int, int);

int counter;
int bufsize;
int *buffer;

int out = 0; // Indice del consumidor.
int in = 0;  // indice del productor.

// Productor y consumidor duermen un número aleatorio en [0, wait_prod] y
// [0, wait_cons] respectivamente.
int wait_prod;
int wait_cons;
int rand_wait;

// Buffer donde se almacena que leyo el consumidor.
int *reader_results;

/* Productor */
static void* producer(void *p) 
{
    int item = 0;  // Item a producir.
    int num = 0;   // Cantidad de items producidos.
    
    while (num++ < counter) {
        // Agrega un item al buffer.
        buffer[in] = item;
        draw(6, in, item);

        // Valor del proximo item.
        item = item + 1;

        // Indice del buffer para el proximo item.
        in = (in + 1) % bufsize; 

        // Duerme
        sleep(rand_wait == 1 ? rand() % wait_prod : wait_prod);
    }
    
    pthread_exit(0);
}

/* Consumidor */
static void* consumer(void *p) 
{
    int num = 0;
    
    while (num < counter) {
        // Lee un item del buffer.
        int item = buffer[out];

        // Actualiza el indicador en pantalla.
        draw(7, out, item);

        // Agrega item en la lista de leidos.
        reader_results[num++] = item;

        // Indice del próximo elemento.
        out = (out + 1) % bufsize;

        // Duerme
        sleep(rand_wait == 1 ? rand() % wait_cons : wait_cons);
    }
    
    pthread_exit(0);
}

/* Datos iniciales en la pantalla. */
static void setup_draw(void)
{
    int i;
    xy(4,1), printf("Buffer:");
    xy(6,1), printf("Prod:");
    xy(7,1), printf("Cons:");

    xy(4,10);
    for(i = 0; i < bufsize; i++) {
        xy(4,10+(i*4)), printf("-");
    }

    xy(6,10), printf("^");
    xy(7,10), printf("^");
    xy(9,1), fflush(stdout);
}

/* Dibuja el símbolo ^ y el valor producido si es el productor. */
static void draw(int w, int i, int v)
{
    xy(w, 10), printf("\033[K");
    xy(w, 10+(i*4));
    printf("^");
    if (w==6) {
        xy(4,10+(i*4));
        printf("%d",v);
    }
    xy(6,1), fflush(stdout);
}

int main(int argc, char** argv) 
{
    int i;
    pthread_t producer_t, consumer_t;

    // Controla argumentos.
    if (argc != 6) {
        fprintf(stderr, "Uso: %s bufsize items wait-prod wait-cons rand\n", argv[0]);
        fprintf(stderr, "\tbufsize: tamaño del búfer.\n");
        fprintf(stderr, "\titems: número de items a producir/consumir.\n");
        fprintf(stderr, "\twait-prod: número de segundos que espera el productor.\n");
        fprintf(stderr, "\twait-cons: número de segundos que espera el consumidor.\n");
        fprintf(stderr, "\trand: si la espera es aleatoria (1) o no (0).\n");
        exit(EXIT_FAILURE);
    }

    // Inicializa semilla para números pseudo-aleatorios.
    srand(getpid());

    clear();  // Borra la pantalla.

    // Tamaño del buffer.
    bufsize = atoi(argv[1]);    
    if (bufsize <= 0) {
        fprintf(stderr, "bufsize tiene que ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    // Reserva memoria para el buffer.
    buffer = (int*) malloc(sizeof(int) * bufsize);
    if (buffer == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    printf("Tamaño del buffer: %d\n", bufsize);

    // Cantidad de items a producir.
    counter = atoi(argv[2]);
    if (counter <= 0) {
        fprintf(stderr, "counter tiene que ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    wait_prod = atoi(argv[3]);
    if (wait_prod <= 0) {
        fprintf(stderr, "wait-prod tiene que ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    wait_cons = atoi(argv[4]);
    if (wait_cons <= 0) {
        fprintf(stderr, "cons-wait tiene que ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    rand_wait = atoi(argv[5]);
    if (rand_wait != 0 && rand_wait != 1) {
        fprintf(stderr, "rand_wait debe ser 0 (no) o 1 (random).\n");
        exit(EXIT_FAILURE);
    }

    printf("Items a producir/consumir: %d\n", counter);

    // Reserva memoria para guardar lo que lee el consumidor.
    reader_results = (int*) malloc(sizeof(int)*counter);

    setup_draw();  // Imprime en pantalla datos iniciales.
    
    // Crea el productor.
    pthread_create(&producer_t, NULL, producer, NULL); 
    // Crea el consumidor.
    pthread_create(&consumer_t, NULL, consumer, NULL); 

    // Espera a que los hilos finalicen.
    pthread_join(producer_t, NULL);  
    pthread_join(consumer_t, NULL);

    // Imprime en pantalla lo que leyó el consumidor.
    xy(9,1), printf("Consumidor leyo: ");
    for (i = 0; i < counter; i++) {
        printf("%d ", reader_results[i]);
    }
    printf("\n");
    
    exit(EXIT_SUCCESS);
}
