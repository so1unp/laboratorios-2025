/**
 * Copiar este archivo en el directorio de xv6, y luego
 * agregarlo en la lista UPROGS del Makefile de xv6.
 */
#include "types.h"
#include "user.h"
#include "fcntl.h"

#define SIZE 2048
char buf[SIZE];

int
main(int argc, char *argv[])
{
    int i = 0;
    int fd;
    int newline = '\n';

    printf(1, "Writing %d bytes to disk.\n", SIZE);

    for (i = 0; i < SIZE; i++) {
        buf[i] = 'a' + (i % 26);
    }

    fd = open("testwrite.bin", O_CREATE | O_WRONLY);
    write(fd, buf, SIZE);
    write(fd, &newline, 1);
    close(fd);

    exit();
}
