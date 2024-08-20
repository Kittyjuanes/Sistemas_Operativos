/**************************************************
Pontificia Universidad Javeriana
Autor: J. Garzon 
Fecha: 23/07/22024
Materia: Sistemas Operativos
Tema: Programacion en C: Primera Parte
Requerimientos:
        - Función que sume 2 enteros
        - Función que calcule el area de una circunferencia
        - Función que de la bienvenida
*****************************************************/

#include <stdio.h>

/** Funcion que da la bienvenida **/
void bienvenida()
{  
  printf("Hola  Mundo... \n");
  printf("\t\t Bienvenidos a mi clase de Sistemas Operativos:) \n");
}

/** Funcion que suma 2 enteros **/
void suma2enteros()
{
  int suma, num1, num2;
  
  printf("Primer entero es:  \n");
  scanf("%d", &num1);
  printf("Segundo entero es: \n");
  scanf("%d", &num2);
  
  suma = num1 + num2;
  
  printf("El resultado es: %d\n", suma);
}

/** Funcion que calcula el area una circunferencia **/
void area_circunferencia()
{
  int radio;
  float pi = 3.14;
  
  printf ("El radio es: \n");
  scanf("%d", &radio);
  
  float area = radio * radio * pi;
  
  printf("El area es: %f\n", area);
}  
  

int main(int argc, char *argv[]){

  bienvenida();
  suma2enteros();
  area_circunferencia();

  return 0;
}