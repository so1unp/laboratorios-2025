# Laboratorio 6 - Entrada / Salida

## Ejercicio 1: Interrupción del teclado

Cuando ejecutamos _xv6_ utilizando el comando `make qemu-nox`, se emula una conexión por medio de una terminal serial (ver Terminales en las filminas de teoría). Al presionar una tecla, se genera una interrupción en la interfaz UART. Para presentar un caracter por pantalla se envía por la misma interfaz.

1. Describir el proceso de E/S en _xv6_ cuando el usuario presiona una tecla del teclado. ¿Se utilizan comandos específicos de E/S o se utiliza E/S mapeada en memoria?

2. Cuando se presiona una tecla, ¿El caracter es inmediatamente enviado al proceso que este leyendo de la entrada estándar? Justificar.

## Ejercicio 2: Comunicación por la pantalla.
Para simular la salida por un monitor conectado a la tarjeta de vídeo en la PC, hay que ejecutar _xv6_ utilizando el comando `make -qemu` (sin el `-nox`).

La función `cgaputc()` escribe el carácter que recibe como parámetro en la memoria de video en modo texto de un adaptador de vídeo CGA. El archivo `cga.pdf` es el manual de referencia de este adaptador. 

Se utilizan los siguientes datos:
- `CRTPORT` es un `#define` con el valor `0x3D4`, que es la **dirección** del puerto de comandos de la controladora de vídeo en el espacio de direcciones de E/S.
- La variable `*crt` es un puntero a la **memoria de video en modo texto**, mapeada en la dirección `0xB8000`. La memoria de vídeo es un arreglo de 4000 bytes que representa una grilla de 80 columnas y 25 filas. Cada carácter requiere 2 bytes, el primero indica el color de la fuente y fondo, mientras que el segundo es el código ASCII.
- La posición del cursor es almacenada por la controladora de vídeo en los registros _Cursor Location High Register_ y _Cursor Location Low Register_ (ver archivo `cga.txt`).

La función `cgaputc()` realiza las siguientes acciones:
1. Obtiene la posición del cursor y lo almacena en la variable `pos`.
3. Si el caracter a mostrar es un `\n` o un `BACKSPACE` actualiza la variable `pos`, que almancena la posición del cursor.
2. En cambio, si el carácter es imprimible, lo escribe en la memoria de vídeo en la posición correspondiente. 
4. Comprueba que la nueva posición del cursor este dentro de los límites de la pantalla.
5. Realiza el _scroll_ (desplazamiento) de la pantalla si corresponde.
6. Actualiza el cursor hardware con la nueva posición.

Responder:
1. Agregar comentarios en el código de la función `cgaputc()` que indiquen si una operación de E/S es mapeada en memoria o utilizar registros especiales de E/S. 
2. Modificar `cgaput.c` para cambiar el color con que el que se imprimen los caracteres por pantalla. Probar también cambiar el color de fondo. Ver la sección *Alphanumeric Modes* (pag. 10) en la documentación CGA (recordar que Intel usa *little-endian*). 

## Ejercicio 3: Disco

En este ejercicio vamos a estudiar como funciona el _driver_ de disco y el sistema de E/S de _xv6_. Usen de referencia el documento _xv6-ide.pdf_ que describe la E/S de disco en _xv6_.

En _xv6_ el disco se organiza de la siguiente manera (ver archivos `fs.h` y `mkfs.c`):
```
 ┌──────┬─────────────┬─────┬─────────┬──────┬──────┐
 │ boot │ super-block │ log │ i-nodes │ free │ data │
 └──────┴─────────────┴─────┴─────────┴──────┴──────┘
```
Donde:
- _boot_: sector con información para el inicio del sistema.
- _super-block_: super bloque del sistema de archivos.
- _log_: conjunto de sectores destinado a realizar _journaling_ del sistema de archivos. Los ignoraremos por el momento.
- _inodes_: sectores que almacenan información de los archivos.
- _free_: sector que contiene informacion de los sectores libres.
- _data_: sectores que contienen los datos de los archivos y directorios.

Copiar el programa `testwrite.c` en el directorio de _xv6_ y agregarlo en el arreglo `UPROGS` del `Makefile`. Modificar el archivo `ide.c` para que imprima por pantalla el número de sector de disco que se esta modificando, ignorando los sectores menores a 32. Una vez realizada la modificación:

1. Iniciar _xv6_ utilizando `make clean && make qemu-nox` (de manera que se genere de nuevo la imagen del disco duro) y ejecutar nuevamente `testwrite`. ¿Qué sectores de disco se están modificando? ¿Cuantos son los sectores que contienen el contenido del archivo generado? Justificar el número.

2. Sin reiniciar _xv6_, ejecutar nuevamente `testwrite`. ¿Qué diferencia hay en la lista de sectores modificados? Justificar.

3. Resumir el ciclo de vida del requerimiento de E/S de `testwrite.c`, explicando las llamadas al sistema utilizadas, que interrupciones ocurren y la secuencia de funciones invocadas por el _kernel_.

4. ¿Qué tipo de política de planificación de disco se utiliza?

5. ¿Se emplea DMA o la operación de E/S es administrada totalmente por el SO?

6. ¿Por que la función `idewait()` realiza un `polling`?

7. Opcional: agregar la llamada al sistema `int rblk(int device, int nblock, char *buf)` que permita leer el contenido del sector de disco indicado. Probar la llamada al sistema usando el programa `rblk.c`, leyendo los primeros bytes de alguno de los bloques de datos generados por la ejecución de `testwrite.c`.

---

¡Fin del Laboratorio!
