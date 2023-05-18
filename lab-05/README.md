# Laboratorio 5 - Comunicación y sincronización entre procesos

## Ejercicio 1: Tuberías

Copiar dentro del directorio de este laboratorio el archivo `sh.c` del Laboratorio 2, e implementar soporte para el uso de tuberías (_pipes_) en el mismo. El objetivo es poder ejecutar comandos como:

```console
$ echo "hola" | wc
    1   1   5
$
```

El _parser_ del intérprete de comandos ya reconoce el operador `|` y guarda en la estructura `pipecmd` todos los datos requeridos para conectar dos procesos mediante una tubería. Deben agregar el código necesario en la función `runcmd()` en la etiqueta `PIPE` del `case`. Las llamadas al sistema que deben utilizar son:

* [`pipe()`](http://man7.org/linux/man-pages/man2/pipe.2.html): crea una tubería.
* [`fork()`](http://man7.org/linux/man-pages/man2/fork.2.html): para crear un nuevo proceso.
* [`close()`](http://man7.org/linux/man-pages/man2/close.2.html): para cerrar un descriptor de archivo.
* [`dup2()`](http://man7.org/linux/man-pages/man2/dup.2.html): para duplicar un descriptor de archivo.

## Ejercicio 2: Paso de mensajes

Vamos a utilizar la librería de mensajes POSIX para desarrollar el programa `msgbox.c`, que permita intercambiar mensajes entre los usuarios del sistema. La estructura de los mensajes esta definida por `msg_t`:

```c
#define USERNAME_SIZE   15
#define TXT_SIZE        100

/**
 * Estructura del mensaje:
 * - sender: nombre del usuario que envió el mensaje.
 * - text: texto del mensaje.
 */
struct msg {
    char sender[USERNAME_SIZE];
    char text[TXT_SIZE];
};

typedef struct msg msg_t;
```

Para crear un buzón y enviar y recibir mensajes, utilizar las siguientes funciones:

* [`mq_open()`](http://man7.org/linux/man-pages/man3/mq_open.3.html): crea una nueva cola de mensajes o abre una ya existente.
* [`mq_send()`](http://man7.org/linux/man-pages/man3/mq_send.3.html): envía un mensaje a la cola de mensajes.
* [`mq_receive()`](http://man7.org/linux/man-pages/man3/mq_receive.3.html): recibe un mensaje.
* [`mq_close()`](http://man7.org/linux/man-pages/man3/mq_close.3.html): cierra el descriptor de una cola de mensajes.
* [`mq_unlink()`](http://man7.org/linux/man-pages/man3/mq_unlink.3.html): elimina una cola de mensajes.
* [`getlogin_r`](https://www.man7.org/linux/man-pages/man3/getlogin.3.html): obtiene el nombre del usuario.

El manual [`mq_overview`](http://man7.org/linux/man-pages/man7/mq_overview.7.html) presenta una introducción general al API de colas de mensajes.

Una vez completado el programa, deben poder crear colas de mensajes y envíar y recibir mensajes por medio de las mismas utilizando el comando `./msgbox`.

## Ejercicio 3: Dashboard

Vamos a armar un espacio de memoria compartida, al que llamaremos _dashboard_, cuyo contenido puede ser modificado por distintos procesos. Este espacio es un arreglo de 50x50 caracteres, sobre el cual se pueden escribir palabras en alguna posición. La estructura de datos que representa el _dashboard_ es `dashboard_t`:

```c
struct dashboard {
    char dashboard[50][50];
};

typedef struct dashboard dashboard_t;
```

Para crear y acceder a un _dashboard_ usaremos el API de POSIX para crear y utilizar segmentos de memoria compartida. Mediante estos segmentos, diferentes procesos pueden intercambiar datos de una manera más rapida que mediante el uso de mensajes. El manual [`shm_overview`](http://man7.org/linux/man-pages/man7/shm_overview.7.html) tiene una introducción al API de memoria compartida de POSIX.

Completar el programa `dashboard.c`, de manera que se puedan crear, eliminar, imprimir y modificar estos _dashboards_. Las principales funciones que vamos a usar son:

* [`shm_open()`](http://man7.org/linux/man-pages/man3/shm_open.3.html): crea un nuevo objeto de memoria compartida, o abre uno ya existente.
* [`ftruncate()`](http://man7.org/linux/man-pages/man2/ftruncate.2.html): cambia ("trunca") el tamaño del segmento de memoria compartida.
* [`mmap()`](http://man7.org/linux/man-pages/man2/mmap.2.html): mapea el segmento de memoria compartida indicado dentro del espacio de direcciones del proceso.
* [`close()`](http://man7.org/linux/man-pages/man2/close.2.html): cierra el descriptor de un segmento de memoria compartida.
* [`shm_unlink()`](http://man7.org/linux/man-pages/man3/shm_unlink.3.html): elimina el segmento de memoria compartida indicado.

## Ejercicio 4: Buffer limitado

El programa `buf.c` implementa un ejemplo de productor-consumidor haciendo uso de un _buffer limitado_. El programa no utiliza mecanismos de sincronización para el acceso al buffer. Esto puede ocasionar condiciones de carrera. Modificar el programa para sincronizar el acceso al buffer, empleando semáforos y _mutexs_. 

Utilizar las siguientes funciones:
* Crear un mutex: [`pthread_mutex_init()`](http://man7.org/linux/man-pages/man3/pthread_mutex_init.3p.html)
* Inicializar el semáforo: [`sem_init()`](https://man7.org/linux/man-pages/man3/sem_init.3.html)
* Tomar un semáforo: [`sem_wait()`](https://man7.org/linux/man-pages/man3/sem_wait.3.html)
* Liberar un semáforo: [`sem_post()`](https://man7.org/linux/man-pages/man3/sem_post.3.html)
* Tomar un mutex: [`pthread_mutex_lock()`](https://www.man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
* Liberar un mutex: [`pthread_mutex_unlock()`](https://www.man7.org/linux/man-pages/man3/pthread_mutex_unlock.3p.html)
* Eliminar un semáforo: [`sem_destroy()`](https://man7.org/linux/man-pages/man3/sem_destroy.3.html)
* Eliminar un mutex: [`pthread_mutex_destroy()`](https://www.man7.org/linux/man-pages/man3/pthread_mutex_destroy.3p.html)

## Ejercicio 5: Criba de Erastóstenes

La criba de Erastóstenes es un método para encontrar todos los números primos menores que un cierto número natural. El método puede ser simulado mediante el siguiente pseudo-código, ejecutando múltiples procesos concurrentemente comunicados mediante tuberías:

```c
p = obtener un número del proceso del lado izquierdo
imprimir p
loop:
    n = obtener un número del proceso del lado izquierdo
    if (p no divide a n):
        enviar n al proceso del lado derecho
```

Implementar el programa en el archivo `primes.c`. Por ejemplo, para calcular los primos entre el 2 y el 11, el proceso inicial ingresa los números en la tubería. Luego, el segundo proceso elimina todos los múltiples de 2 y pasa el resto de los números al proceso siguiente. El tercer proceso elimina los múltiplos de 3, el cuarto los múltiplos de 5 y así sucesivamente:

```
               Proceso 2            Proceso 3            Proceso 4            Proceso 5            Proceso 6
             ┌───────────┐        ┌───────────┐        ┌───────────┐        ┌───────────┐        ┌───────────┐
    2  ──────► Imprime 2 │        │           │        │           │        │           │        │           │
             │           │        │           │        │           │        │           │        │           │
    3  ──────►           │ ──────►│ Imprime 3 │        │           │        │           │        │           │
             │           │        │           │        │           │        │           │        │           │
    4  ──────► Descarta  │        │           │        │           │        │           │        │           │
             │           │        │           │        │           │        │           │        │           │
    5  ──────►           │ ──────►│           │ ──────►│ Imprime 5 │        │           │        │           │
             │           │        │           │        │           │        │           │        │           │
    6  ──────► Descarta  │        │           │        │           │        │           │        │           │
             │           │        │           │        │           │        │           │        │           │
    7  ──────►           │ ──────►│           │ ──────►│           │ ──────►│ Imprime 7 │        │           │
             │           │        │           │        │           │        │           │        │           │
    8  ──────► Descarta  │        │           │        │           │        │           │        │           │
             │           │        │           │        │           │        │           │        │           │
    9  ──────►           │ ──────►│ Descarta  │        │           │        │           │        │           │
             │           │        │           │        │           │        │           │        │           │
   10  ──────► Descarta  │        │           │        │           │        │           │        │           │
             │           │        │           │        │           │        │           │        │           │
   11  ──────►           │ ──────►│           │ ──────►│           │ ──────►│           │ ──────►│ Imprime 11│
             └───────────┘        └───────────┘        └───────────┘        └───────────┘        └───────────┘
```


---

¡Fin del Laboratorio 5!
