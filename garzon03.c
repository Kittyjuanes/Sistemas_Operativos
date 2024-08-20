/*****************************************************
        Autor: Juan Esteban Garzón Tinjaca
        Fecha: 30/07/2024
        Profesor: Jhon Corredor
        Materia: Sistemas Operativos
        Tema: Argumentos de entrada en c
****************************************************/

#include <stdio.h>


int main(int argc, char *argv[]){
  
  int x = 102;
  int *p = x;
  int vector[]= {5,10,20,30,40,50,80};
  int *p1 = vector;
  int *p2 = vector+20;
  int diferencia =p2 - p1;
 
  //1
        printf("Ha ingresado [%d]: \n\n;", argc);
  //2
        printf("Ha ingresado la edad: %s \n\n;", argv[1]);
  //3
        printf("Ha ingresado el mes: %s \n\n", argv[2]);
  //4
        printf("\n El valor X es de %d \n:", x);
        printf("\La direccion de X es de\n: %p \n\n", p);
      
  //5
        printf("\n El valor p1 es de: %p \n", p1);
        printf("\n El valor p2 es de: %p \n", p2);
        printf("\n El valor diferencia es de: %d \n", diferencia);

        printf("\n");
        return 0;

}