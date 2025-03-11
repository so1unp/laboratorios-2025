# Laboratorio 1 - Llamadas al sistema y el Lenguaje de programación C
El objetivo de este laboratorio es familiarizarse con el lenguaje C y las herramientas de desarrollo.

### Programa de referencia
El programa `demo.c` demuestra las funcionalidades básicas del lenguaje C: declaración de variables, estructuras, ciclos, control, punteros e imprimir mensajes por la salida estándar haciendo uso de funciones de la biblioteca.

## Ejercicio 1
Modificar el programa `sum.c` para que retorne la suma de los números enteros indicados desde la línea de comandos. Por ejemplo:
```
$ bin/sum 1 2 3
6
$
```
Utilizar la funcion [`atoi()`](https://www.man7.org/linux/man-pages/man3/atoi.3.html) para convertir las cadenas de texto a números enteros.

## Ejercicio 2
Completar el programa `palabras.c` para que imprima las palabras que recibe desde la _entrada estándar_. Por ejemplo:
```
$ bin/palabras
Esto es un texto ↵
Esto
es
un
texto
Hola mundo ↵
Hola
mundo
^C
$
```
En el ejemplo, luego de ingresar una frase y presionar Enter (`↵`), el programa imprime cada palabra, una por línea. El programa finaliza al presionar `^C` (Ctrl+C).

Utilizar la función [`getchar()`](https://www.man7.org/linux/man-pages/man3/getchar.3.html) para obtener un carácter desde la _entrada estándar_.

## Ejercicio 3
Ejecutar el programa `sum.c` mediante el comando `strace` como se indica a continuación, para obtener las llamadas al sistema que utiliza durante su ejecución:

```console
$ strace bin/sum 1 2 3 > /dev/null
```

**Nota**: `> /dev/null` redirije la _salida estándar_ de `bin/hola` al archivo especial del sistema `/dev/null`, que descarta todo lo que se escriba en el mismo. De esta manera evitamos que la salida del comando `hola` se mezcle con la de `strace`.

Responder:

1. ¿Cuantas llamadas al sistema invoca el programa?
2. ¿Cúales son las llamadas al sistema que invocan las funciones de biblioteca `puts()` y `exit()`?
3. Describir los parámetros que utiliza la llamada al sistema invocada por `puts()`.

## Ejercicio 4
Completar los programas `encrypt.c` y `decrypt.c`, para que encripten y desencripten un mensaje respectivamente, utilizando la técnica *Least Significant Bit* (LSB).

### Llamadas al sistema a utilizar

Utilizar las siguientes llamadas al sistema:

- [`open()`](http://man7.org/linux/man-pages/man2/open.2.html)
- [`write()`](http://man7.org/linux/man-pages/man2/write.2.html)
- [`read()`](http://man7.org/linux/man-pages/man2/read.2.html)
- [`fstat()`](http://man7.org/linux/man-pages/man2/fstat.2.html)
- [`close()`](http://man7.org/linux/man-pages/man2/close.2.html)

### Encriptador
El programa `encrypt.c` debe leer el mensaje a encriptar como un argumento desde la línea de comando y generar el contenido encriptado en la salida estándar o bien guardarlo en el archivo indicado mediante la opción `-o`. Utilizar `argv` y `argc` para determinar donde debe escribirse el resultado. Cada caracter del mensaje es almacenado en el bit menos significativo de cada byte. Por ejemplo, el mensaje "hola" requiere de 32 bytes: 4 caracteres cada uno de 8 bits (1 byte).

### Desencriptador
El programa `decrypt.c` debe leer los datos encriptados desde la entrada estándar o desde el archivo indicado mediante la opción `-i` y presentar el mensaje oculto en la salida estándar. Recordar que el tamaño del archivo encriptado (o el número de bytes leído de la entrada estándar) indica la longitud del mensaje oculto.

## Ejercicio 5: ejecución de xv6
En el repositorio `xv6` van a encontrar el código del sistema operativo _xv6_, una versión muy reducida de Unix. Parados en dicho directorio, pueden ejecutarlo utilizando la maquina virtual *QEMU* ejecutando el comando `make qemu-nox`. Verían algo similar a lo siguiente en la consola:

```console
Booting from Hard Disk..xv6...
cpu0: starting 0
sb: size 1000 nblocks 941 ninodes 200 nlog 30 logstart 2 inodestart 32 bmap start 58
init: starting sh
$
```

Si ejecutamos un comando como `ls` veremos un error ya que no está implementada la ejecución de comandos:
```console
$ ls
exec not implemented
$
```

Para terminar la ejecución de QEMU, presionar `C^A x` (esto es, `Ctrl+A` y luego la tecla `x`).

---

¡Fín del Laboratorio 1!
