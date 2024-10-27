/********************************************************************************
* Fecha: 23 agosto 2024
* Autor: Juan Esteban Garzon Tinjaca
* Materia: Sistemas Operativos
* Tema: Gestión de memoria
* Tópico: Función realloc(), se crea ya sea un calloc() o un malloc(), debido a que realloc() 
actualiza el espacio de memoria ya reservado. Si realloc() actualiza a un tamaño mayor
se añade memoria al final, pero si el tamaño es menor, los datos se perderán.
******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


int main(){

  int *arr;

  arr = (int *) malloc(100 * sizeof(int)); //Se asigna la memoria para 10 enteros

  if (arr == NULL) {
    printf("Error al asignar memoria\n");
    return 1;
  }

  //Se asigna valores a la memoria reservada
  for (int i = 0; i < 100; i++){
    arr[i] = (i + 4)*2;   //Se suma por 4 y multiplica por 2 para variar los valores del arreglo
    printf("Memoria asignada en la posición [%d] el valor es de = %d\n", i,  arr[i]);
  }


  // Reducir el tamaño del bloque a 50 valores
  int *new_arr = realloc(arr, 50 * sizeof(int));


  if (new_arr == NULL) {
      printf("Error al asignar memoria\n");
      return 1;
      // No sobrescribir 'ptr' porque sigue siendo válido.
  } 
  
  else {
      // 'new_ptr' ahora es válido, y 'ptr' ya no debe ser usado.
      arr = new_arr;

      //Se asigna valores a la memoria reservada
      for (int i = 0; i < 50; i++){
        arr[i] = (i + 4)*2;   //Se suma por 4 y multiplica por 2 para variar los valores del arreglo
        printf("Memoria asignada en la posición [%d] el valor es de = %d\n", i,  arr[i]);
      }
  }



  return 0;
}
