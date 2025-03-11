# Laboratorio 1 - El Lenguaje de Programación C

El objetivo de este laboratorio es familiarizarse con el lenguaje C y las herramientas de desarrollo.

## Ejercicio 0
El programa `demo.c` es una demostración de las funcionalidades básicas del lenguaje C: declaración de variables, estructuras, ciclos, control, punteros e imprimir mensajes por la salida estándar haciendo uso de funciones de la biblioteca. Leer el código, compilar el programa y ejecutarlo.

## Ejercicio 1
Completar el programa `hola.c` para que imprima por la salida estandar el mensaje indicado como parámetro. Utilizar la función [`printf()`](https://www.man7.org/linux/man-pages/man3/printf.3.html) para imprimir el mensaje. Un ejemplo de ejecución es:
```
$ bin/hola "hola mundo"
hola mundo
$
```

Para obtener el texto a imprimir se debe utilizar el argumento `argv[]` de la función `main()`. No olvidar verificar que exista el argumento inspeccionando `argc`.

## Ejercicio 2
Modificar el programa `sum.c` para que retorne la suma de un número arbitrario de enteros indicados desde la línea de comandos. Por ejemplo:
```
$ bin/sum 1 2 3
6
$
```
Utilizar la funcion [`atoi()`](https://www.man7.org/linux/man-pages/man3/atoi.3.html) para convertir las cadenas de texto a números enteros.

## Ejercicio 3
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

## Ejercicio 4
Completar el programa `histograma.c` para que imprima un histograma de las longitudes de las palabras de su entrada. La versión más sencilla del histograma es con las barras horizontales. La orientación vertical es más desafiante. El texto a analizar se debe leer desde la *entrada estándar*. Un ejemplo de ejecución sería:
```
$ bin/histograma
Esto es un texto ↵
Esto es otro texto ↵
^D
1
2 ***
3
4 ***
5 **
$

``` 
En el ejemplo se ejecuta el programa, que queda a la espera de datos desde la entrada estándar. Se ingresan las frases `Esto es un texto` y `Esto es otro texto`, cada una seguida de un *Enter*. Luego se presiona `^D` (Ctrl+D), combinación que envía un `EOF`.

El histograma indica que en el texto analizado se encontraron tres palabras con dos letras (*es* dos veces y *un*), tres palabras de cuatro letras y dos de cinco.  

---

¡Fín del Laboratorio 1!
