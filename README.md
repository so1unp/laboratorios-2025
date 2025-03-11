# Laboratorios Sistemas Operativos

## Herramientas de desarrollo
Como editor de texto se utilizará VSCode y se empleará el compilador desde la línea de comandos.  Para el control de versiones se empleará [Git](https://git-scm.com/) y GitHub para administrar los repositorios remotos. [Aquí](http://rogerdudler.github.io/git-guide/index.es.html) pueden leer una guía sencilla para aprender los comandos básicos de Git.

## Compilación
Para compilar los programas se utiliza el comando `make`. Si se ejecuta sin parámetros, compila _todos_ los programas del laboratorio. Si se indica el nombre de un archivo _sin la extensión_ compila sólo el programa indicado.

Por ejemplo, para compilar el ejercicio 1 (programa `sum.c`):
```
$ make sum
gcc -g -Wall -Wextra -Wshadow -Wconversion -Wunreachable-code -o ./bin/sum sum.c
sum.c: In function ‘main’:
sum.c:4:14: warning: unused parameter ‘argc’ [-Wunused-parameter]
    4 | int main(int argc, char *argv[])
      |          ~~~~^~~~
sum.c:4:26: warning: unused parameter ‘argv’ [-Wunused-parameter]
    4 | int main(int argc, char *argv[])
      |                    ~~~~~~^~~~~~
$
```

El compilador genera una serie de _warnings_ (advertencias). En este caso, nos está indicando que los parámetros `argc` y `argv` de la función `main()` no estan siendo utilizados (`unused parameter`). El objetivo es que la resolución de cada ejercicio cumpla con el enunciado y que _además_ compile *sin advertencias*.

El ejecutable se crea dentro del subdirectorio `bin` del laboratorio. Lo podemos comprobar haciendo un listado del contenido de dicho directorio:
```
$ ls bin
sum 
$
```
Para ejecutar el programa recien compilado:
```
$ bin/sum
$
```
Como todavía no se realizó una modifación al programa, no realiza ninguna acción.

## Laboratorios
- Lab0X: laboratorio.
- xv6: código fuente de xv6.

## xv6
En el directorio `xv6` van a encontrar el código del sistema operativo _xv6_, una versión muy reducida de Unix. Parados en dicho directorio, pueden ejecutarlo utilizando la maquina virtual *QEMU* ejecutando el comando `make qemu-nox`. Verían algo similar a lo siguiente en la consola:

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

