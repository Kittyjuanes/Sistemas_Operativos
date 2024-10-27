/**************************************************************************************************
* Fecha: 13 agosto 2024
* Autor: Juan Esteban Garzon
* Materia: Sistemas Operativos
* Tema: Gestión de memoria
* Tópico: Reserva de memoria con la función calloc()
* La función calloc reserva un espacio de memoria de tamaño requerido.
* Los espacios son de igual tamaño y de memoria contigua.
* La asignación es dinámica y útil para estructuras de datos complejas.
* Además, los bloques de memoria asignados se inicializan en 0.
* Devuelve un puntero señalando el inicio del espacio de memoria reservado.
*************************************************************************************************/


#include <stdio.h>
#include <stdlib.h>

int main() {
    // Declarar variables y punteros
    int *p1 = calloc(4, sizeof(int)); // Asignación de vector de tamaño 4
    int (*p2)[4] = calloc(10, sizeof(int[4])); 
    int *p3 = calloc(5, sizeof(*p3)); // Bloque de memoria de 5 enteros


    for (int i = 0; i < 10; i++) {
        p2[i][0] = i; // Asignar el valor en la primera columna de cada fila
        printf("El valor en la 1ra columna de la fila %d es %d\n", i, p2[i][0]); // Imprimir el valor en la columna 1 de la fila actual
    }

    // LIBERACIÓN DE MEMORIA
    free(p1);
    free(p2);
    free(p3);

    return 0;
}