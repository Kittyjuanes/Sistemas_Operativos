/**************************************************************************************************
* Fecha: 13 agosto 2024
* Autor: Juan Esteban Garzon
* Materia: Sistemas Operativos
* Tema: Gestión de memoria
* Tópico: Reserva de memoria con la función malloc()
* La función malloc reserva un espacio de memoria de tamaño requerido.
* devuelve un puntero señalando el inicio del espacio de memoria reservado.
*************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main() {
  // Declarar variables y punteros
  int *bloque00 = malloc(4 * sizeof(int)); // Asignación de vector de tamaño 4
  int *puntero = malloc(15 * sizeof(*puntero)); // Bloque de memoria para 15 enteros

  // Verificar si la memoria esta correcta
  if (bloque00 == NULL || puntero == NULL) {
    printf("Error al asignar memoria\n");
    return 1;
  }

  // Llenar espacios del primer bloque reservado
  for (int i = 0; i < 4; i++) { 
    bloque00[i] = i * 2;
    printf("El valor de memoria reservada Bloque00[%d] = %d\n", i, bloque00[i]);
  }

  // Llenar espacios del segundo bloque reservado
  if (puntero != NULL) {
    *(puntero + 8) = 9889; // Se llena la posición 9con valor de 9889
    printf("El valor en la 9na posición del puntero es %d\n", *(puntero + 8));
    printf("El valor en la 9na posición del puntero es %d\n", puntero[8]);
  }

  // LIBERACIÓN DE MEMORIA
  free(bloque00);
  free(puntero);

  return 0;
}