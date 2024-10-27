/********************************************************************
* Pontificia Universidad Javeriana  
* Autor: Felipe Andrés Garrido Flores
* Fecha: 1 de Octubre de 2024
* Materia: Sistemas Operativos
* Tema: fork_NamedPipes
                *********     Servidor Cliente    *********
********************************************************************/

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>  

#define FIFO_FILE "/tmp/fifo_twoway"

void reverse_string(char *);

int main() {
    int fd;
    char readbuf[80];
    char end[10];
    int to_end;
    int read_bytes;

    //Creacion de fifo con sus debidos permisos
    mkfifo(FIFO_FILE, S_IFIFO | 0640);
    strcpy(end, "fin");
    fd = open(FIFO_FILE, O_RDWR);

    while (1) {
         //Abrir el fifo en modo lectura
        read_bytes = read(fd, readbuf, sizeof(readbuf));
        readbuf[read_bytes] = '\0';
        printf("Servidor: String recibido: \"%s\" y su longitud es de %d\n", readbuf, (int)strlen(readbuf));

        //verificacion la finalizacion de la cadena
        to_end = strcmp(readbuf, end);

        if (to_end == 0) {
            close(fd); //termina si recibe fin
            break;
        }

        //Invertir la cadena
        reverse_string(readbuf);
        printf("Servidor: String recibido: \"%s\" y la longitud es de %d\n", readbuf, (int)strlen(readbuf));
        //Escribir la cadena invertida en el fifo
        if (write(fd, readbuf, strlen(readbuf)) == -1) {
            perror("Error");
        }
        sleep(2); //espera 2 segundos para que no se imprima demasiado rápido
    }
    return 0;
}


//Función para voletar lo que hay dentro del buff
void reverse_string(char *str) {
    int last, limit, first;
    char temp;
    last = strlen(str) - 1;
    limit = last / 2;
    first = 0;

    while (first < last) {
        temp = str[first];
        str[first] = str[last];
        str[last] = temp;
        first++;
        last--;
    }
    return;
}
