/***************************************************************************************
* Pontificia Universidad Javeriana
* Autores: Felipe Garrido y Juan Garzón
* Fecha: 22 de Octubre de 2024
* Materia: Sistemas Operativos
* Tema: Proyecto SO
*  Descripción:
*  Este programa implementa un sistema de comunicación mediante tuberías FIFO,
*  permitiendo que un publicador envíe noticias a suscriptores.
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX_CHAR 100              // Máximo número de caracteres por mensaje
#define MAX_SUBSCRIPTORES 10      // Máximo número de suscriptores
#define MAX_TOPICS 5              // Máximo número de temas que un suscriptor puede manejar

// Estructura para almacenar la información de un suscriptor
typedef struct {
    char temas[MAX_TOPICS][MAX_CHAR];  // Arreglo para almacenar los temas de interés
    int num_temas;  // Número actual de temas que el suscriptor sigue
} Suscriptor;

// Semáforo para proteger el acceso a la lista de suscriptores
sem_t sem_suscriptores;  

// Función que valida si el tema de la noticia coincide con los temas que el suscriptor sigue
int tema_valido(char tema_noticia, Suscriptor suscriptor) {
    // Compara el primer carácter del tema de la noticia con los temas del suscriptor
    for (int i = 0; i < suscriptor.num_temas; i++) {
        if (suscriptor.temas[i][0] == tema_noticia) {
            return 1;  // Si hay coincidencia, retorna 1 (verdadero)
        }
    }
    return 0;  // Si no hay coincidencia, retorna 0 (falso)
}

// Función para procesar las suscripciones de los suscriptores
void procesar_suscripciones(char *pipeSSC, Suscriptor suscriptores[], int *num_suscriptores) {
    int pipe_fd = open(pipeSSC, O_RDONLY);  // Abrir el pipe para recibir las suscripciones
    if (pipe_fd == -1) {
        perror("Error al abrir el pipe de suscripciones");
        exit(EXIT_FAILURE);  // Si no se puede abrir el pipe, termina el programa
    }

    printf("Entra a la función de procesamiento de suscripciones\n");

    Suscriptor nuevo_suscriptor = {0};  // Inicializar un nuevo suscriptor vacío
    char buffer[MAX_CHAR];

    // Leer la línea con los temas desde el pipe
    if (read(pipe_fd, buffer, sizeof(buffer)) > 0) {
        // Eliminar salto de línea si existe al final de la cadena
        buffer[strcspn(buffer, "\n")] = '\0';

        // Verificar si el buffer tiene algún contenido
        if (strlen(buffer) > 0) {
            // Separar los temas recibidos por comas
            char *tema = strtok(buffer, ",");
            while (tema != NULL) {
                // Eliminar posibles espacios al inicio del tema
                while (*tema == ' ') tema++;

                // Agregar el tema al suscriptor si hay espacio
                if (nuevo_suscriptor.num_temas < MAX_TOPICS) {
                    strncpy(nuevo_suscriptor.temas[nuevo_suscriptor.num_temas++], tema, MAX_CHAR);
                    printf("Tema recibido: '%s'\n", tema);
                }
                tema = strtok(NULL, ",");  // Obtener el siguiente tema
            }

            // Almacenar el nuevo suscriptor en la lista de suscriptores
            sem_wait(&sem_suscriptores);  // Adquirir el semáforo para proteger el acceso a la lista
            suscriptores[*num_suscriptores] = nuevo_suscriptor;
            (*num_suscriptores)++;  // Incrementar el contador de suscriptores
            sem_post(&sem_suscriptores);  // Liberar el semáforo
        } else {
            printf("No se recibió ningún tema para suscribir.\n");
        }
    }

    printf("Numero de suscriptores.%d\n: ", *num_suscriptores);
    close(pipe_fd);  // Cerrar el pipe de suscripciones
}

// Función para procesar las noticias recibidas desde el publicador y enviarlas a los suscriptores
void procesar_noticias(char *pipePSC, char *pipeSSC, Suscriptor suscriptores[], int num_suscriptores) {
    int pipe_fd = open(pipePSC, O_RDONLY);  // Abrir el pipe para recibir las noticias
    if (pipe_fd == -1) {
        perror("Error al abrir el pipe de noticias");
        exit(EXIT_FAILURE);  // Si no se puede abrir el pipe, termina el programa
    }

    int sub_fd = open(pipeSSC, O_WRONLY);  // Abrir el pipe de salida para enviar las noticias a los suscriptores
    if (sub_fd == -1) {
        perror("Error al abrir el pipe de salida para los suscriptores");
        close(pipe_fd);
        exit(EXIT_FAILURE);  // Si no se puede abrir el pipe, termina el programa
    }

    char buffer[MAX_CHAR];
    while (read(pipe_fd, buffer, sizeof(buffer)) > 0) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Eliminar el salto de línea al final de la noticia
        printf("Noticia recibida: %s\n", buffer);

        char tema_noticia = buffer[0];  // Extraer el primer carácter del buffer como tema de la noticia
        // Enviar la noticia a los suscriptores que estén interesados en este tema
        for (int i = 0; i < num_suscriptores; i++) {
            printf("Comparando tema_noticia '%c' con tema del suscriptor '%c'\n", tema_noticia, suscriptores[i].temas[0][0]);
            if (tema_valido(tema_noticia, suscriptores[i])) {  // Compara el tema con los temas de cada suscriptor
                // Enviar la noticia al suscriptor
                if (write(sub_fd, buffer, strlen(buffer)) == -1) {
                    perror("Error al enviar noticia a suscriptor");
                }
                // Enviar un salto de línea adicional al suscriptor
                if (write(sub_fd, "\n", 1) == -1) {
                    perror("Error al enviar salto de línea");
                }
                break;  // Solo enviar la noticia a un suscriptor
            }
        }
    }

    close(pipe_fd);  // Cerrar el pipe de noticias
    close(sub_fd);   // Cerrar el pipe de salida para suscriptores
}

int main(int argc, char *argv[]) {
    char *pipePSC = NULL, *pipeSSC = NULL;  // Pipes para recibir noticias y suscripciones
    int timeF = 0;  // Tiempo de finalización (en segundos)

    // Procesar los argumentos de la línea de comandos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) pipePSC = argv[i + 1];  // Pipe para recibir noticias
        else if (strcmp(argv[i], "-s") == 0) pipeSSC = argv[i + 1];  // Pipe para recibir suscripciones
        else if (strcmp(argv[i], "-t") == 0) timeF = atoi(argv[i + 1]);  // Tiempo de espera
    }

    // Verificar que los parámetros necesarios estén presentes
    if (pipePSC == NULL || pipeSSC == NULL || timeF <= 0) {
        fprintf(stderr, "Uso: %s -p pipePSC -s pipeSSC -t timeF\n", argv[0]);
        exit(EXIT_FAILURE);  // Terminar si los parámetros no son válidos
    }

    // Inicialización del semáforo para proteger el acceso a la lista de suscriptores
    if (sem_init(&sem_suscriptores, 0, 1) != 0) {
        perror("Error al inicializar el semáforo");
        exit(EXIT_FAILURE);
    }

    Suscriptor suscriptores[MAX_SUBSCRIPTORES] = {0};  // Arreglo de suscriptores
    int num_suscriptores = 0;  // Contador de suscriptores

    // Procesar suscripciones y noticias
    procesar_suscripciones(pipeSSC, suscriptores, &num_suscriptores);
    procesar_noticias(pipePSC, pipeSSC, suscriptores, num_suscriptores);

    // Esperar el tiempo especificado antes de finalizar
    sleep(timeF);
    printf("Sistema finalizando...\n");

    // Destruir el semáforo
    sem_destroy(&sem_suscriptores);

    return 0;  // Finalizar el programa
}
