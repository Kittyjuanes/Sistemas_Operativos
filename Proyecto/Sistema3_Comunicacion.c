#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX_CHAR 100
#define MAX_SUBSCRIPTORES 10
#define MAX_TOPICS 5

typedef struct {
    char temas[MAX_TOPICS][MAX_CHAR];
    int num_temas;
} Suscriptor;

sem_t sem_suscriptores;  // Semáforo para proteger el acceso al arreglo de suscriptores

int tema_valido(char tema_noticia, Suscriptor suscriptor) {
    for (int i = 0; i < suscriptor.num_temas; i++) {
        
        if (suscriptor.temas[i][0] == tema_noticia) {
            return 1;  // El tema coincide
        }
    }
    return 0;  // El tema no coincide
}


void procesar_suscripciones(char *pipeSSC, Suscriptor suscriptores[], int *num_suscriptores) {
    int pipe_fd = open(pipeSSC, O_RDONLY);
    if (pipe_fd == -1) {
        perror("Error al abrir el pipe de suscripciones");
        exit(EXIT_FAILURE);
    }
    printf("Entra a la función de procesamiento de suscripciones\n");
    
    Suscriptor nuevo_suscriptor = {0};
    char buffer[MAX_CHAR];

    // Lee la línea de temas una vez
    if (read(pipe_fd, buffer, sizeof(buffer)) > 0) {
        // Elimina el salto de línea final, si existe
        buffer[strcspn(buffer, "\n")] = '\0';

        // Verifica si el buffer tiene contenido
        if (strlen(buffer) > 0) {
            // Separar los temas por comas
            char *tema = strtok(buffer, ",");
            while (tema != NULL) {
                // Eliminar posibles espacios adicionales al inicio
                while (*tema == ' ') tema++;

                // Agregar el tema al suscriptor si hay espacio
                if (nuevo_suscriptor.num_temas < MAX_TOPICS) {
                    strncpy(nuevo_suscriptor.temas[nuevo_suscriptor.num_temas++], tema, MAX_CHAR);
                    printf("Tema recibido: '%s'\n", tema);
                }
                tema = strtok(NULL, ",");  // Obtener el siguiente tema
            }

            // Almacenar el suscriptor en la lista compartida
            sem_wait(&sem_suscriptores);
            suscriptores[*num_suscriptores] = nuevo_suscriptor;
            (*num_suscriptores)++; 
            sem_post(&sem_suscriptores);
        } else {
            printf("No se recibió ningún tema para suscribir.\n");
        }
    }
  
    printf("Numero de suscriptores.%d\n: ", *num_suscriptores);
    close(pipe_fd);
}


void procesar_noticias(char *pipePSC, char *pipeSSC, Suscriptor suscriptores[], int num_suscriptores) {
    int pipe_fd = open(pipePSC, O_RDONLY);
    if (pipe_fd == -1) {
        perror("Error al abrir el pipe de noticias");
        exit(EXIT_FAILURE);
    }

    int sub_fd = open(pipeSSC, O_WRONLY);  // Mantén abierto el pipe de suscripción una vez
    if (sub_fd == -1) {
        perror("Error al abrir el pipe de salida para los suscriptores");
        close(pipe_fd);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_CHAR];
    while (read(pipe_fd, buffer, sizeof(buffer)) > 0) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Eliminar salto de línea
        printf("Noticia recibida: %s\n", buffer);
   
        char tema_noticia = buffer[0];  // La letra que indica el tema
        // Enviar la noticia a suscriptores que coincidan con el tema
        for (int i = 0; i < num_suscriptores; i++) {
          printf("Comparando tema_noticia '%c' con tema del suscriptor '%c'\n", tema_noticia, suscriptores->temas[i][0]);
            if (tema_valido(tema_noticia, suscriptores[i])) {
                if (write(sub_fd, buffer, strlen(buffer)) == -1) {
                    perror("Error al enviar noticia a suscriptor");
                }
                if (write(sub_fd, "\n", 1) == -1) {
                    perror("Error al enviar salto de línea");
                }
                break;
            }
        }
    }

    close(pipe_fd);
    close(sub_fd);
}

int main(int argc, char *argv[]) {
    char *pipePSC = NULL, *pipeSSC = NULL;
    int timeF = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) pipePSC = argv[i + 1];
        else if (strcmp(argv[i], "-s") == 0) pipeSSC = argv[i + 1];
        else if (strcmp(argv[i], "-t") == 0) timeF = atoi(argv[i + 1]);
    }

    if (pipePSC == NULL || pipeSSC == NULL || timeF <= 0) {
        fprintf(stderr, "Uso: %s -p pipePSC -s pipeSSC -t timeF\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Inicialización del semáforo
    if (sem_init(&sem_suscriptores, 0, 1) != 0) {
        perror("Error al inicializar el semáforo");
        exit(EXIT_FAILURE);
    }

    Suscriptor suscriptores[MAX_SUBSCRIPTORES] = {0};
    int num_suscriptores = 0;

    procesar_suscripciones(pipeSSC, suscriptores, &num_suscriptores);
    procesar_noticias(pipePSC, pipeSSC, suscriptores, num_suscriptores);

    sleep(timeF);
    printf("Sistema finalizando...\n");

    // Destrucción del semáforo
    sem_destroy(&sem_suscriptores);

    return 0;
}
