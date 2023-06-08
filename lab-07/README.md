# Laboratorio 7 - Administración de Memoria

Consultar el apunte de práctica sobre administración de memoria en _xv6_ y el Capítulo 2 "Page Tables" del libro de _xv6_. Las respuestas a los ejercicios deben realizarse en un documento en formato PDF.

## Ejercicio 1: mapa de memoria de un proceso

En este ejercicio usaremos la utilidad `pmap` para estudiar como esta distribuido en memoria un proceso en ejecución. Usaremos el programa `maps.c` que utiliza `fork()` para crear un proceso hijo. Ambos procesos esperan recibir dos señales `SIGUSR1`. De esta manera el proceso queda bloqueado a la espera de la señal y nos permite estudiar como estan mapeados en memoria.

### Actividades

* Compilar y ejecutar el programa `maps.c` en una terminal y desde otra terminal, ejecutar el comando `pmap -p $(pgrep maps)`. Como resultado debe mostrar los resultados de ambos procesos (padre e hijo). Notar que las direcciones de memoria son idénticas en ambos procesos. ¿Por qué?

* Ejecutar `pmap -p $(pgrep maps) | grep maps`, para que se muestren únicamente los mapeos correspondientes al ejecutable de nuestro programa. ¿Por qué una de las páginas esta marcada como de ejecución? ¿Por qué otra de las páginas permite la escritura?

* Ejecutar `pmap -p -XX $(pgrep maps) > maps.txt`. Este comando muestra mucha más información, por lo que redirigimos la salida al archivo `maps.txt` para leer más comodamente los resultados en un editor de texto. Abrir el archivo `maps.txt` y verificar que el mapeo del _heap_ y de la página marcada como de lectura/escritura en el punto anterior, estan compartidas, según indica la columna `Shared_Dirty`. ¿Por qué?

* Eliminar ambos procesos ejecutando `kill -9 $(pgrep maps)`. Modificar el código de `mem.c` de manera tal que el proceso hijo modifique la variable global `a` y también el _heap_. Ejecutar luego nuevamente `maps` en una terminal, y desde otra repetir el punto anterior (tendría que mostrar identicos resultados). Luego, ejecutar `kill -s SIGUSR1 $(pgrep maps)` de manera que ambos procesos continuan su ejecución. Ejecutar luego `pmap -p -XX $(pgrep maps) >> maps.txt` (importante: notar el `>>` para que agregue el contenido al final del archivo, en lugar de reemplazarlo). Verificar que la columna `Shared_Dirty` del _heap_ y de la pagina marcada como de escritura son cero en ambos procesos. ¿Por qué?

## Ejercicio 2: Contador de páginas
Agregar a _xv6_ una llamada al sistema `pgcnt()`, que retorne la cantidad de páginas virtuales que ocupa actualmente el proceso que invoca la llamada al sistema. El código de la llamada al sistema es el siguiente (lo pueden encontrar también en el archivo `sys_pgcnt.c`):
```
int
sys_pgcnt(void)
{
    pde_t *pde;  // page directory
    pte_t *pte;  // page table

    int i = 0;
    int j = 0;
    int cnt = 0;

    unsigned int flag = PTE_P | PTE_U;

    for(i=0; i < NPDENTRIES; i++)
        pde = &((myproc()->pgdir)[i]);

        if(*pde & PTE_P) {
            pte = (pte_t*) P2V(PTE_ADDR(*pde));

            for(j=0; j < NPTENTRIES; j++) {
                if((pte[j] & flag) == flag) {
                   cnt = cnt + 1;
                }
            }
        }
    }

    return cnt;
}
```

El archivo `pgcnt.c` es un programa de usuario que invoca la llamada al sistema `pgcnt()`. Deben agregarlo a la lista `UPROGS` en el `Makefile` de _xv6_.

Este programa invoca la llamada al sistema `pgcnt()` tres veces. La primera al inicio del programa y luego de invocar a `sbrk` y `malloc`:
```
$ pgcnt
2
3
11
$
```

### Responder:
* Explicar el resultado del programa, justificando el número de páginas que cada invocación a `pgcnt()` retorna. ¿Por qué la diferencia entre las invocaciones de `sbrk` y `malloc`?

## Ejercicio 3: Paginación bajo demanda
Muchos programas reservan memoria que pueden no utilizar nunca. Por ejemplo, un arreglo de gran tamaño del que no se hace uso. El sistema operativo puede posponer la asignación efectiva de memoria a estas secciones hasta que sean requeridas para lectura y/o escritura. Cuando un proceso quiere acceder a una de estas secciones de memoria aún no asignadas, ocurre un _page fault_ (fallo de página) y el sistema operativo debe entonces cargar las páginas requeridas desde disco o bien reservar la memoria solicitada. En este ejercicio vamos a implementar un esquema de paginación bajo demanda rudimentario en _xv6_.

### Parte 1
Para incrementar el tamaño del segmento de datos se utiliza la llamada al sistema `sbrk()`, implementada en la función `sys_sbrk()` en el archivo `sysproc.c`.

Comentar la llamada a `growproc()`, que es la función que invoca a `sys_sbrk()`. Hecha esta modificación, compilar y ejecutar _xv6_ y ejecutar el comando `echo hola`.
- ¿Qué error aparece? ¿A qué tipo de error se refiere el número indicado por `trap`?

### Parte 2
Modificar el código en el archivo `trap.c`, para que ante un _page fault_ producido por un programa a nivel de usuario, realice la carga de la página correspondiente a la dirección de memoria virtual que causó el _page fault_. No se necesitan cubrir todos los casos posibles de error, debe bastar para ejecutar comando simples como `echo` o `ls`.

El código a agregar en `trap.c` lo pueden encontrar en el archivo `pgflt.txt`. Como se invoca a `mappages()`, hay que eliminar la sentencia `static` de su declaración en el archivo `vm.c`, y agregar su prototipo en `trap.c`. Además, en la función `sys_sbrk()`, se debe actualizar el tamaño del proceso (`myproc()->sz`), en función del argumento `n`.

### Responder:
* ¿Donde en particular, se debe agregar el código en `trap.c`?
* Probar el correcto funcionamiento de las modificaciones.
* Explicar, línea por línea, el código del archivo `pgflt.txt`, agregado a `trap.c`.

---

¡Fín del Laboratorio!
