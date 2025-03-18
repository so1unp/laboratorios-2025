# Laboratorio 3 - Planificación

## Ejercicio 1

Una señal es una notificación enviada a un proceso de que ocurrió un evento. Son análogas a una interrupción de hardware e interrumpen el normal funcionamiento del programa. Las señales pueden ser generadas por el kernel para indicar que ocurrió una excepción de hardware, que el usuario tecleo una combinación especial de carácteres (por ejemplo `^C`) o que ocurrio un evento de software (por ejemplo, un proceso hijo finalizó su ejecución). Un programa puede enviar señales a otros procesos o a si mismo. Una lista completa de las señales se puede consultar en [`signals`](https://man7.org/linux/man-pages/man7/signal.7.html).

En este ejercicio vamos a practicar como tratar las señales que llegan a un proceso. Por defecto, la acción que se ejecuta al llegar una señal es la de finalizar el proceso (en la mayoría de los casos) o ignorar la señal.

Completar el programas `sig_receiver.c` de manera que espere por la recepción de señales. Por cada señal que recibe, debe imprimir el identificador de la señal y una descripción de la misma.

Para enviar señales a `sig_receiver.c` vamos a utilizar el comando [`kill`](http://man7.org/linux/man-pages/man1/kill.1.html). Por ejemplo, para enviar las señales 15 (`SIGTERM`) y 8 (`SIGFPE`):
```
$ bin/sig_receiver &
[1] 27309
$ kill -15 27309 
15: Terminated
$ kill -8 27309
8: Floating point exception
$
```

Utilizar las siguientes funciones y llamadas al sistema:

* [`sigaction()`](http://man7.org/linux/man-pages/man2/sigaction.2.html): permite cambiar la acción tomada por el proceso al recibir una señal.
* [`pause()`](http://man7.org/linux/man-pages/man2/pause.2.html): espera a recibir una señal.
* [`strsignal()`](http://man7.org/linux/man-pages/man3/strsignal.3.html): retorna un puntero a una cadena con la descripción de la señal indicada.
* El archivo de cabecera `signal.h` incluye la constante `NSIG` que es igual al número de señales del sistema más uno.

Responder:

1. ¿Cuáles son las señales que no es posible atrapar? ¿Por qué?

## Ejercicio 2: creación de procesos
El programa `forkprio.c` debe crear un cierto número de procesos hijos, cada uno ejecutando con un valor de prioridad progresivamente más bajo. Luego de un cierto número de segundos se debe envíar una señal de terminación (`SIGTERM`) a todos los procesos hijos. Estos, antes de terminar su ejecución, deben imprimir su PID, prioridad y el número de segundos que ejecutaron.


El siguiente esquema muestra como es la relación con los procesos hijos:
```
      ┌──────────┐                             fork()
      │ forkprio ├───────────┬───────────┬────────────────────┐
      └─────┬────┘           │           │                    │
            │                │           │                    │
            │                │           │                    │
 sleep(secs)│           ┌────▼───┐   ┌───▼────┐          ┌────▼───┐
            .           │ Hijo 1 │   │ Hijo 2 │   ...    │ Hijo n │
            .           └────▲───┘   └───▲────┘          └────▲───┘
            .                │           │                    │
            │                │           │                    │
            │                │           │                    │
            ├────────────────┴───────────┴────────────────────┘
            │                                 kill(SIGTERM)
            │
            ▼
          exit()
```

Requisitos:

- El primer argumento en la linea de comandos debe ser el número de procesos hijos a crear. El segundo parámetro el número de segundos que deben ejecutar. El tercer parámetro indica si se reducen las prioridades (1) o no (0).
- Si el número de segundos es 0 (cero) se debe ejecutar indefinidamente.
- El formato que deben utilizar los procesos hijos al imprimir es `"Child %d (nice %2d):\t%3li\n"` (pid, prioridad, tiempo de ejecución).
- La función `busywork()` en el archivo `forkprio.c` tiene un bucle infinito que repetidamente invoca la función `times()` para consumir tiempo de CPU. Es la función que deben invocar los procesos hijos.

Utilizar las siguientes llamadas al sistema:

* [`setpriority()`](https://man7.org/linux/man-pages/man2/setpriority.2.html): cambia la prioridad de un proceso (también se puede utilizar [`nice()`](https://man7.org/linux/man-pages/man2/nice.2.html)).
* [`getpriority()`](https://man7.org/linux/man-pages/man2/setpriority.2.html): retorna el valor de prioridad un proceso.
* [`getrusage()`](https://man7.org/linux/man-pages/man2/getrusage.2.html): obtiene estadísticas de ejecución de un proceso.
* [`sleep()`](https://man7.org/linux/man-pages/man3/sleep.3.html): suspende la ejecución de un proceso durante el número indicado de segundos.
* [`kill()`](http://man7.org/linux/man-pages/man2/kill.2.html): envía una señal a un proceso.
* [`sigaction()`](http://man7.org/linux/man-pages/man2/sigaction.2.html): permite modificar la acción a ejecutar al recibir una señal.

Para verificar que efectivamente se estan creando los procesos hijos, pueden realizar la siguiente prueba:
```console
$ bin/forkprio 5 0 0 >/dev/null &
$ pstree -cp $(pgrep forkprio | head -1)
$ pkill forkprio
```

Responder:

1. Ejecutar `bin/forkprio 1 0 1 >/dev/null &`. Luego ejecutar el comando `ps -fp $(pgrep forkprio)`. ¿En que estados están los procesos? Explicar.
2. Ejecutar `bin/forkprio 1 10 1` en tu computadora y luego en el servidor de la cátedra ¿Existe alguna diferencia en el resultado? Ejecutar múltiples veces para estar seguro de los resultados.
3. Ejecutar `bin/forkprio 10 30 1 | sort -k 4 -h` y describir el resultado de la ejecución. ¿Por qué el total del tiempo ejecutado por los procesos hijos puede ser mayor que el tiempo que espera el proceso padre?
4. Si el comando anterior se ejecuta indicando que no se cambien las prioridades de los procesos hijos, ¿Cúal es el resultado? Explicar por qué.

## Ejercicio 3: planificación con hilos 
Completar el programa `sched-threads.c` para que cree una cierta cantidad de hilos que escriben en un buffer compartido. Este buffer es un arreglo de enteros, donde cada hilo escribe su identificador una cierta cantidad de veces. Al finalizar el programa se imprime este arreglo, de manera que se observa el orden de ejecución de los hilos. Se debe poder especificar como algoritmos de planificación `SCHED_FIFO`, `SCHED_RR` y `SCHED_OTHER`.

Por ejemplo, una vez implementado el programa, una ejecución que cree tres hilos, cada uno escribiendo tres items en el buffer, planificados mediante FIFO, debería dar un resultado similar al siguiente:
```
$ sudo ./bin/sched-threads -F 3 3
0 0 0 1 1 1 2 2 2
$
```

Notar que se debe ejecutar el comando con `sudo` ya que se necesitan mayores privilegios para utilizar las políticas `SCHED_RR` y `SCHED_FIFO`.

## Ejercicio 3: getppid() en xv6

Añadir a _xv6_ la llamada al sistema `getppid()`, para obtener el PID del proceso padre. Utilizar como guía la llamada al sistema `getpid()`. Agregar también un programa de nombre `testppid.c` que pruebe el funcionamiento de la nueva llamada al sistema.

## Ejercicio 4: contador de procesos en xv6

Añadir a _xv6_ la llamada al sistema `pscnt()`, que retorne el número de procesos actualmente el sistema. El prototipo de la función para el usuario es: `int pscnt(void)`. Añadir un programa de nombre `pscnt.c` para pruebe el funcionamiento de esta llamada al sistema creando procesos hijos.

---

¡Fin del Laboratorio 3!
