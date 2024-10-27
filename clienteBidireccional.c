/********************************************************************
* Pontificia Universidad Javeriana  
* Autor: Felipe Andrés Garrido Flores
* Fecha: 1 de Octubre de 2024
* Materia: Sistemas Operativos
* Tema: fork_NamedPipes
                *********     Cliente Servidor     *********
********************************************************************/

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>  

// Ruta del FIFO
#define FIFO_FILE "/tmp/fifo_twoway"

int main() {
    int fd;          
    int end_process;      
    int stringlen;       
    int read_bytes;         
    char readbuf[80]; 
    char end_str[5];         

    printf("FIFO_Cliente: Enviar mensajes, indefinidamente, hasta tener un 'fin' \"fin\"\n");

    // Abrir el FIFO en modo lectura/escritura con permisos 0666
    fd = open(FIFO_FILE, O_CREAT | O_RDWR, 0666);
    strcpy(end_str, "fin"); // Si se escribe 'fin', el bucle termina

    while (1) {
        printf("Escriba el String: ");
        // Leer la entrada del usuario
        if (fgets(readbuf, sizeof(readbuf), stdin) == NULL) {
            perror("Error al leer la entrada");
        }

        // Obtener la longitud de la cadena leída y reemplazar el salto de línea por el carácter nulo
        stringlen = strlen(readbuf);
        readbuf[stringlen - 1] = '\0';
        // Comparar la entrada con la cadena de fin
        end_process = strcmp(readbuf, end_str);

        if (end_process != 0) {
            // Escribir en el FIFO si no es la cadena de fin
            if (write(fd, readbuf, strlen(readbuf)) == -1) {
                perror("Error al escribir");
            }
            printf("FIFOCLIENT: Cadena enviada: \"%s\" y la longitud es de %d\n", readbuf, (int)strlen(readbuf));
            // Leer respuesta del FIFO
            read_bytes = read(fd, readbuf, sizeof(readbuf));
            readbuf[read_bytes] = '\0'; // Añadir el carácter nulo al final
            printf("FIFOCLIENT: Cadena recibida: \"%s\" y la longitud es %d\n", readbuf, (int)strlen(readbuf));
        } else {
            // Escribir en el FIFO la cadena de fin
            if (write(fd, readbuf, strlen(readbuf)) == -1) {
                perror("Error al escribir");
            }
            printf("FIFOCLIENT: Cadena enviada: \"%s\" y la longitud es de %d\n", readbuf, (int)strlen(readbuf));
            close(fd); // Cerrar el FIFO
            break; // Salir del bucle
        }
    }
    return 0; 
}
