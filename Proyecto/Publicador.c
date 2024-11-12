/***************************************************************************************
* Pontificia Universidad Javeriana
* Autores: Felipe Garrido y Juan Garzón
* Fecha: 22 de Octubre de 2024
* Materia: Sistemas Operativos
* Tema: Proyecto SO

* Descripción: Publicador
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#define MAX_CHAR 100

int validar_formato_noticia(char *line) {
    int len = strlen(line);
    if (len < 4) return 0;
    if (!isupper(line[0]) || line[1] != ':' || line[len - 2] != '.' || line[len - 1] != '\n') return 0;
    return 1;
}

void publicar_noticias(char *pipePSC, char *nombre_archivo, int timeN) {
    int pipe_fd;
    char line[MAX_CHAR];
    mkfifo(pipePSC, 0666);
  
    pipe_fd = open(pipePSC, O_WRONLY);
    if (pipe_fd == -1){
        perror("Error al abrir el pipe");
        exit(EXIT_FAILURE);
    }

    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL){
        perror("No se pudo abrir el archivo");
        exit(EXIT_FAILURE);
    }
  
    while (fgets(line, sizeof(line), archivo)) {
        if (!validar_formato_noticia(line)) {
            printf("Formato Incorrecto: %s", line);
            continue;
        }

        printf("\nEnviando noticia: %s\n", line);
        if (write(pipe_fd, line, strlen(line)) == -1) {
            perror("Error al escribir en el pipe");
            break;
        }
        sleep(timeN);  
    }

    close(pipe_fd);
    fclose(archivo);
}

int main(int argc, char *argv[]){
    char *pipePSC = NULL;
    char *nombre_archivo = NULL;
    int timeN = 0;

    for (int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-p") == 0) pipePSC = argv[i+1];
        else if (strcmp(argv[i], "-f") == 0) nombre_archivo = argv[i+1];
        else if (strcmp(argv[i], "-t") == 0) timeN = atoi(argv[i+1]);
    }

    if (pipePSC == NULL || nombre_archivo == NULL || timeN <= 0) {
        fprintf(stderr, "Uso: %s -p pipePSC -f file -t timeN\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    publicar_noticias(pipePSC, nombre_archivo, timeN);
    return 0;
}
