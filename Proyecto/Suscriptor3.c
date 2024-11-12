/***************************************************************************************
* Pontificia Universidad Javeriana
* Autores: Felipe Garrido y Juan Garzón
* Fecha: 22 de Octubre de 2024
* Materia: Sistemas Operativos
* Tema: Proyecto SO

* Descripción: Suscriptor
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_CHAR 100
#define MAX_TOPICS 5

void suscribirse_a_temas(char *pipeSSC) {
    mkfifo(pipeSSC, 0666);

    int pipe_fd = open(pipeSSC, O_WRONLY);
    if (pipe_fd == -1) {
        perror("Error al abrir el pipe de suscriptor para escritura");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Ingrese sus temas de interés separados por comas (Arte(A), Politica(P), Ciencia(C), Espectaculo(E), Sucesos(S)):\n");
        char tema[MAX_CHAR];
        fgets(tema, sizeof(tema), stdin);

        // Elimina el salto de línea
        tema[strcspn(tema, "\n")] = '\0';

        if (tema[0] == '\0') {
            printf("Debe suscribirse a al menos un tema antes de terminar.\n");
            continue;
        }

        // Envía el tema al pipe y verifica que fue exitoso
        if (write(pipe_fd, tema, strlen(tema) + 1) == -1) {
            perror("Error al enviar los temas");
            close(pipe_fd);
            exit(EXIT_FAILURE);
        }

        // Si la escritura fue exitosa y el tema no está vacío, salir del ciclo
        break;
    }


    close(pipe_fd);
}




void recibir_noticias(char *pipeSSC) {
    int pipe_fd = open(pipeSSC, O_RDONLY);
    if (pipe_fd == -1) {
        perror("Error al abrir el pipe para recibir noticias");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_CHAR];
    printf("Esperando noticias...\n");

    while (read(pipe_fd, buffer, sizeof(buffer)) > 0) {
        printf("Noticia recibida: %s\n", buffer);
        memset(buffer, 0, sizeof(buffer));
    }

    close(pipe_fd);
}

int main(int argc, char *argv[]) {
    char *pipeSSC = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            pipeSSC = argv[i + 1];
        }
    }

    if (pipeSSC == NULL) {
        fprintf(stderr, "Uso: %s -s pipeSSC\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    suscribirse_a_temas(pipeSSC);
    recibir_noticias(pipeSSC);

    return 0;
}

