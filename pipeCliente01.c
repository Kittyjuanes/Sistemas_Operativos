/********************************************************************
* Pontificia Universidad Javeriana  
* Autor: Felipe Andrés Garrido Flores
* Fecha: 24 de septiembre de 2024
* Materia: Sistemas Operativos
* Tema: Named Pipes
********************************************************************/

/*
Este programa implementa un cliente de una named pipe, que permite la comunicación entre procesos. La aplicación cliente pide mensajes y los envía a través de una FIFO hasta que se ingrese la palabra end, y este termina el proces
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define FIFO_FILE "MYFIFO" 

int main() { 
    int fd;
    int end_process;
    int stringlen;
    char readbuf[80];
    char end_str[5];

    printf("FIFO_CLIENT: Enviando mensaje, infinatemente, para leer INR \"end\"\n");

    //crear fifo
    mkfifo(FIFO_FILE, 0640);


    //abrir fifo en modo escritura
    fd = open(FIFO_FILE, O_CREAT | O_WRONLY, 0640);
    strcpy(end_str, "end");

    //bucle para los mensajes
    while (1) {
        printf("Ingrese Mensaje: ");
        if (fgets(readbuf, sizeof(readbuf), stdin) == NULL) {
            perror("Error en fgets");
            continue; 
        } //manejo de errores

        //obtener el tamaño del mensaje
        stringlen = strlen(readbuf);
        readbuf[stringlen - 1] = '\0'; 

        end_process = strcmp(readbuf, end_str);

        // sie el proceso no es end seguir
        if (end_process != 0) {
            if (write(fd, readbuf, strlen(readbuf)) == -1) {
                perror("Error en write");
                continue;
            }
            printf("Mensaje enviado: \"%s\" y tamaño del string %d\n", readbuf, (int)strlen(readbuf));
        } else {
            if (write(fd, readbuf, strlen(readbuf)) == -1) {
                perror("Error en write");
                continue; 
            }
            printf("Mensaje enviado: \"%s\" y tamaño del string %d\n", readbuf, (int)strlen(readbuf));
            close(fd); //cerrar el file
            break;
        }
    }
    return 0;
}
