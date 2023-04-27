# Laboratorio 4 - Hilos y Planificación

## Ejercicio 1

En este ejercicio vamos a comparar el costo de la creación de hilos con el de creación de procesos.

El programa `benchmark.c` crea la cantidad de procesos o de hilos indicada en la línea de comandos. El programa ya cuenta con la funcionalidad de procesos implementanda. Por ejemplo, el siguiente comando crea 1000 procesos hijos (opción `-p`): 
```sh
$ bin/benchmark -p 1000
```

Cada vez que crea un proceso, espera a que termine antes de crear el siguiente. Se debe implementar el código necesario para crear hilos (opción `-t`). Utilizar las siguientes funciones:

* [`pthread_create()`](http://man7.org/linux/man-pages/man3/pthread_create.3.html): crea un hilo.
* [`pthread_join()`](http://man7.org/linux/man-pages/man3/pthread_join.3.html): espera a que el hilo indicado finalice.
* [`pthread_exit()`](http://man7.org/linux/man-pages/man3/pthread_exit.3.html): finaliza la ejecución de un hilo.

Una vez implementada la opción de generación de hilos, se debe comparar los tiempos de ejecución de ambas opciones utilizando el comando [`time`](http://man7.org/linux/man-pages/man1/time.1.html):

```sh
$ /usr/bin/time -p bin/benchmark -p 1000
$ /usr/bin/time -p bin/benchmark -t 1000
```

**Nota**: es importante usar el _path_ completo (`/usr/bin/time`) para no ejecutar el comando `time` provisto por el _shell_. 

Responder:

1. ¿Cual de las dos variantes tuvo menos costo, la creación de hilos o la creación de procesos? Justificar.

## Ejercicio 2

Completar el programa [`threads.c`](threads.c) para que cree *n* hilos que esperan durante una cierta cantidad de segundos:

* El número *n* debe ser indicado como parámetro en la línea de comandos.
* Cada hilo debe tener asignado un número entero como _id_ único: el primer hilo debe tener el _id_ 1, el segundo el _id_ 2 y así sucesivamente.
* Cada hilo debe esperar un número aleatorio de segundos no mayor al parámetro indicado en la línea de comandos, antes de terminar. Utilizar la función [`sleep()`](http://man7.org/linux/man-pages/man3/sleep.3.html).
* Al finalizar, cada hilo debe retornar el número de segundos que durmió con [`pthread_exit()`](http://man7.org/linux/man-pages/man3/pthread_exit.3.html).
* El hilo `main` debe esperar a que el resto de los hilos finalicen. Luego debe imprimir cuantos segundos durmió cada hilo. Este dato se obtiene mediante [`pthread_join()`](http://man7.org/linux/man-pages/man3/pthread_join.3.html).

Por ejemplo, si se ejecuta el programa indicando que se creen 3 hilos que duerman como máximo 5 segundos, debe obtenerse una salida similar a la siguiente:

```bash
$ bin/threads 3 5
Hilo 0: dormire 3
Hilo 2: dormire 1
Hilo 1: dormire 2
Hilo 0 termino: 3
Hilo 1 termino: 2
Hilo 2 termino: 1
$
```

Para crear y administrar los hilos, utilizar las siguientes funciones:

* [`pthread_create()`](http://man7.org/linux/man-pages/man3/pthread_create.3.html): crea un hilo.
* [`pthread_join()`](http://man7.org/linux/man-pages/man3/pthread_join.3.html): espera a que el hilo indicado finalice.
* [`pthread_self()`](http://man7.org/linux/man-pages/man3/pthread_self.3.html): para obtener el _id_ del hilo actualmente en ejecución.
* [`pthread_exit()`](http://man7.org/linux/man-pages/man3/pthread_exit.3.html): finaliza la ejecución de un hilo.

Responder:

1. ¿Que sucede si `main()` no invoca `pthred_join()` antes de ejecutar `exit()`? Justificar.
2. Sin agregar la invocación a `pthread_join()`, reemplazar en `main()` la invocación a `exit()` por `pthread_exit()`. ¿Qué sucede ahora? Justificar.
3. Reemplazar `pthread_exit()` en la función `main()` por `return 0`. ¿Que sucede? Justificar. 

## Ejercicio 3

El programa `glob.c` debe crear dos hilos que incrementan la variable global `glob` el número de veces indicado en la línea de comandos. La función que deben invocar ambos hilos es `increment_glob()`.

1. Completar el programa y ejecutarlo, probando distintos valores hasta que se encuentre una _condición de carrera_. ¿Por qué ocurre esta situación de carrera? ¿Cómo se podría evitar?
2. ¿Por qué al utilizar un valor pequeño como parámetro el resultado suele ser correcto?
3. Evitar la condición de carrera mediante el uso de un _semaforo_ (`sem_t`):
    - Para inicializar el semáforo, utilizar la función [`sem_init()`](https://man7.org/linux/man-pages/man3/sem_init.3.html)
    - Para tomar el semáforo utilizar [`sem_wait()`](https://man7.org/linux/man-pages/man3/sem_wait.3.html)
    - Para liberar el semáforo utilizar [`sem_post()`](https://man7.org/linux/man-pages/man3/sem_post.3.html)
    - El semáforo se puede eliminar usando [`sem_destroy()`](https://man7.org/linux/man-pages/man3/sem_destroy.3.html)
4. La sección crítica puede estar dentro del ciclo `for` o bien ser el ciclo `for` completo. Comparar cuánto tiempo tardan en ejecutar ambas variantes, utilizando el comando `/usr/bin/time`. ¿Cuál es la causa de las diferencias?

## Ejercicio 4

En este ejercicio se pide complentar el programa `sched-threads.c` para que cree una cierta cantidad de hilos que escriben en un buffer compartido. Este buffer es un arreglo de enteros, donde cada hilo escribe su identificador una cierta cantidad de veces. Al finalizar el programa se imprime este arreglo, de manera que se observa el orden de ejecución de los hilos.

Por ejemplo, una vez implementado el programa, una ejecución que cree tres hilos, cada uno escribiendo tres items en el buffer, planificados mediante FIFO, debería dar un resultado similar al siguiente:
```
$ sudo ./sched-threads -F 3 3
0 0 0 1 1 1 2 2 2
$
```

Notar que se debe ejecutar el comando con `sudo` ya que se necesitan mayores privilegios para utilizar las políticas `SCHED_RR` y `SCHED_FIFO`.

## Ejercicio 5: hilos en xv6

En este ejercicio vamos a implementar hilos a nivel de usuario en _xv6_. En el directorio de _xv6_ van a encontrar estos archivos:

* `uthread.c`: es un programa de prueba que utiliza la librería de hilos a nivel de usuario.

* `uthreadlib.c` y `uthreadlib.h`: contienen la implementación de una librería de hilos a nivel de usuario.

* `uthread_switch.S`: contiene el código en ensamblador para realizar el cambio de contexto entre dos hilos.

Para probar ejecutar _xv6_ y luego invocar el programa `uthread`. Al ejecutarlo, tiene que indicar que todos los hilos finalizaron:

```sh
xv6...
cpu0: starting 0
sb: size 1000 nblocks 941 ninodes 200 nlog 30 logstart 2 inodestart 32 bmap start 58
init: starting sh
$ uthread 2 1
all threads ended.
$
```

Como puede verse en la ejecución, ningun hilo ejecutó realmente ya que falta implementar el cambio de contexto en el archivo `uthread_switch.S`.

### Cambio de contexto

En `uthreadlib.c` hay dos variables `current_thread` y `next_thread`, que son punteros a una estructura de tipo `thread`. Esta estructura contiene la pila de un hilo y una copia de su puntero a pila (`sp` o _stack pointer_). Para realizar el cambio de contexto, la función `uthread_switch` debe guardar el estado del hilo actual en `current_thread`, restaurar el estado del hilo indicado en `next_thread` y hacer que `current_thread` apunte a `next_thread`.

En el cambio de contexto de un hilo a otro tiene se tienen que realizar las siguientes acciones:

1. Guardar el estado de la CPU del hilo actual en la pila, ccon la instrucción `pushal`. Esta instrucción almacena los registros `EAX`, `EBX`, `ECX`, `EDX`, `ESP`, `EBP`, `ESI`, `EDI` en la pila.

2. Almacenar el puntero a pila actual (`current_thread->sp = esp`).

3. Actualizar el registro `ESP` para que apunte a la pila del hilo a ejecutar (`esp = next_thread->sp`).

4. Actualizar el valor de `current_thread` para que apunte al hilo a ejecutar.

5. Asignar cero a la variable `next_thread`.

6. Restaurar el contexto del hilo a ejecutar con la instrucción `popal`, que restaura en los registros de la CPU los valores que almaceno una invocación previa de `pushal`.

7. Realizar el _return_ de la función, con la instrucción `ret`. De esta manera, se continua con la ejecución del hilo seleccionado.

Una ejecución correcta de `uthread.c` tendría que dar como resultado:

```bash
$ uthread 2 5
my thread running
Thread 0x2D68: 0
my thread running
Thread 0x4D70: 0
Thread 0x2D68: 1
Thread 0x4D70: 1
Thread 0x2D68: 2
Thread 0x4D70: 2
Thread 0x2D68: 3
Thread 0x4D70: 3
Thread 0x2D68: 4
Thread 0x4D70: 4
my thread: exit
my thread: exit
thread_schedule: no more runnable threads
all threads ended.
$
```

## Responder

1. Completar el código de cambio de contexto en el archivo `uthread_switch.S` y comprobar que el programa `uthread.c` ejecute correctamente.

2. ¿Por qué basta con ejecutar `popa` para continuar con la ejecución de `next_thread`?

3. ¿Qué tipo de política de planificación se implementa? Justificar.

4. La implementación tiene una limitación: si ejecutamos más de dos hilos, el tercero no se ejecuta hasta que finalicen los dos primeros. Indicar por qué sucede esto y modificar el código para solucionarlo.

5. Modificar el código para que cuando no existan más hilos para ejecutar, se retome el hilo `main`.

---

¡Fin del Laboratorio 4!
