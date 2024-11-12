/***************************************************************************************
* Pontificia Universidad Javeriana
* Autores: Felipe Garrido y Juan Garzón
* Fecha: 22 de Octubre de 2024
* Materia: Sistemas Operativos
* Tema: Proyecto SO
* Descripción:
*  Implementa un suscriptor que se conecta a un sistema de noticias mediante
*  tuberías FIFO. Permite seleccionar categorías de noticias y recibir solo
*  aquellas que corresponden a las categorías seleccionadas.
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_CHAR 100  // Define el tamaño máximo de las cadenas de caracteres.
#define MAX_TOPICS 5  // Define el número máximo de temas que un suscriptor puede seleccionar.


// Función que permite al suscriptor elegir los temas a los que se quiere suscribir.
void suscribirse_a_temas(char *pipeSSC) {
    // Crea una tubería FIFO (named pipe) para la suscripción.
    mkfifo(pipeSSC, 0666);

    // Abre el pipe de suscripción en modo escritura.
    int pipe_fd = open(pipeSSC, O_WRONLY);
    if (pipe_fd == -1) {  // Si no se puede abrir el pipe, se muestra un error y se termina el programa.
        perror("Error al abrir el pipe de suscriptor para escritura");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Solicita al usuario que ingrese los temas de interés, separados por comas.
        printf("Ingrese sus temas de interés separados por comas (Arte(A), Politica(P), Ciencia(C), Espectaculo(E), Sucesos(S)):\n");
        char tema[MAX_CHAR];
        fgets(tema, sizeof(tema), stdin);  // Lee la entrada del usuario.

        // Elimina el salto de línea al final de la cadena de texto.
        tema[strcspn(tema, "\n")] = '\0';

        // Si el usuario no ingresó ningún tema, solicita que ingrese al menos uno.
        if (tema[0] == '\0') {
            printf("Debe suscribirse a al menos un tema antes de terminar.\n");
            continue;  // Vuelve al inicio del ciclo para pedir al usuario que ingrese un tema válido.
        }

        // Envia los temas al pipe.
        if (write(pipe_fd, tema, strlen(tema) + 1) == -1) {  // El +1 es para incluir el terminador de cadena '\0'.
            perror("Error al enviar los temas");
            close(pipe_fd);  // Cierra el pipe en caso de error.
            exit(EXIT_FAILURE);
        }

        // Si los temas se enviaron correctamente, sale del ciclo.
        break;
    }

    close(pipe_fd);  // Cierra el pipe después de enviar los temas.
}


// Función que recibe las noticias desde el pipe y las muestra en pantalla.
void recibir_noticias(char *pipeSSC) {
    // Abre el pipe de suscripción en modo lectura.
    int pipe_fd = open(pipeSSC, O_RDONLY);
    if (pipe_fd == -1) {  // Si no se puede abrir el pipe para lectura, se muestra un error y se termina el programa.
        perror("Error al abrir el pipe para recibir noticias");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_CHAR];
    printf("Esperando noticias...\n");

    // Lee las noticias del pipe hasta que se termine de leer.
    while (read(pipe_fd, buffer, sizeof(buffer)) > 0) {
        // Muestra las noticias recibidas.
        printf("Noticia recibida: %s\n", buffer);
        memset(buffer, 0, sizeof(buffer));  // Limpia el buffer después de mostrar la noticia.
    }

    close(pipe_fd);  // Cierra el pipe después de recibir todas las noticias.
}

// Función principal que maneja la ejecución del programa.
int main(int argc, char *argv[]) {
    char *pipeSSC = NULL;  // Variable para almacenar la ruta del pipe.

    // Procesa los argumentos de la línea de comandos.
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {  // Si se encuentra la opción -s, asigna el valor al pipeSSC.
            pipeSSC = argv[i + 1];
        }
    }

    // Verifica que se haya recibido el pipe por línea de comandos.
    if (pipeSSC == NULL) {
        fprintf(stderr, "Uso: %s -s pipeSSC\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Llama a las funciones de suscripción y recepción de noticias.
    suscribirse_a_temas(pipeSSC);
    recibir_noticias(pipeSSC);

    return 0;
}
