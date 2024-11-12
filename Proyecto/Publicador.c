/***************************************************************************************
* Pontificia Universidad Javeriana
* Autores: Felipe Garrido y Juan Garzón
* Fecha: 22 de Octubre de 2024
* Materia: Sistemas Operativos
* Tema: Proyecto SO
* Descripción:
* Este programa actúa como un publicador de noticias, leyendo cada línea de un
* archivo y enviándola a través de un pipe a los suscriptores.
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#define MAX_CHAR 100  // Definir el tamaño máximo para las cadenas de caracteres

// Función para validar el formato de la noticia (debe ser de la forma: 'A: noticia.')
int validar_formato_noticia(char *line) {
    int len = strlen(line);
    
    // Verifica que la longitud mínima sea 4 y cumple con el formato esperado
    if (len < 4) return 0;  // La noticia debe ser al menos de 4 caracteres
    if (!isupper(line[0]) || line[1] != ':' || line[len - 2] != '.' || line[len - 1] != '\n') return 0;
    
    // Si cumple con el formato, retorna 1
    return 1;
}

// Función para publicar las noticias leyendo desde un archivo y enviándolas por el pipe
void publicar_noticias(char *pipePSC, char *nombre_archivo, int timeN) {
    int pipe_fd;
    char line[MAX_CHAR];

    // Crear el pipe si no existe
    mkfifo(pipePSC, 0666);

    // Abrir el pipe para escritura
    pipe_fd = open(pipePSC, O_WRONLY);
    if (pipe_fd == -1){
        perror("Error al abrir el pipe");
        exit(EXIT_FAILURE);  // Terminar el programa si no se puede abrir el pipe
    }

    // Abrir el archivo que contiene las noticias
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL){
        perror("No se pudo abrir el archivo");
        exit(EXIT_FAILURE);  // Terminar el programa si no se puede abrir el archivo
    }

    // Leer cada línea del archivo
    while (fgets(line, sizeof(line), archivo)) {
        // Validar el formato de la noticia
        if (!validar_formato_noticia(line)) {
            printf("Formato Incorrecto: %s", line);  // Informar si el formato no es válido
            continue;  // Continuar con la siguiente línea si el formato es incorrecto
        }

        // Imprimir la noticia que se enviará
        printf("\nEnviando noticia: %s\n", line);

        // Enviar la noticia por el pipe
        if (write(pipe_fd, line, strlen(line)) == -1) {
            perror("Error al escribir en el pipe");
            break;  // Terminar si hay un error al escribir en el pipe
        }

        // Esperar el tiempo especificado antes de enviar la siguiente noticia
        sleep(timeN);
    }

    // Cerrar el pipe y el archivo después de finalizar el envío
    close(pipe_fd);
    fclose(archivo);
}

// Función principal del programa
int main(int argc, char *argv[]){
    char *pipePSC = NULL;
    char *nombre_archivo = NULL;
    int timeN = 0;

    // Procesar los argumentos pasados por línea de comandos
    for (int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-p") == 0) pipePSC = argv[i+1];  // Pipe de salida
        else if (strcmp(argv[i], "-f") == 0) nombre_archivo = argv[i+1];  // Archivo con las noticias
        else if (strcmp(argv[i], "-t") == 0) timeN = atoi(argv[i+1]);  // Tiempo de espera entre noticias
    }

    // Verificar que los parámetros necesarios han sido proporcionados
    if (pipePSC == NULL || nombre_archivo == NULL || timeN <= 0) {
        fprintf(stderr, "Uso: %s -p pipePSC -f file -t timeN\n", argv[0]);
        exit(EXIT_FAILURE);  // Terminar si los parámetros no son válidos
    }

    // Llamar a la función que publica las noticias
    publicar_noticias(pipePSC, nombre_archivo, timeN);
    
    return 0;  // Terminar el programa correctamente
}
