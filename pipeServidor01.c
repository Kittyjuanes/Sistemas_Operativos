/********************************************************************
* Pontificia Universidad Javeriana  
* Autor: Felipe Andrés Garrido Flores
* Fecha: 24 de septiembre de 2024
* Materia: Sistemas Operativos
* Tema: Named Pipes
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_FILE "MYFIFO"
  int main() {
     int fd;
     char readbuf[80];
     char end[10];
     int to_end;
     int read_bytes;
     
     //crear fifo con permisos
     mknod(FIFO_FILE, S_IFIFO|0640, 0);
     strcpy(end, "end");

     //bucle para ir leyendo los mensajes
     while(1) {
        //abrir fifo para lectura
        fd = open(FIFO_FILE, O_RDONLY);       
        //leer el fifo
        read_bytes = read(fd, readbuf, sizeof(readbuf));       
        readbuf[read_bytes] = '\0';        
        printf("Cadena recibida: \"%s\" y su tamaño es %d\n", readbuf, (int)strlen(readbuf)); //imprimir cadena
        to_end = strcmp(readbuf, end);
        if (to_end == 0) {
           close(fd); //cerrar el file
           break;
        }
     }
     return 0;
  }
