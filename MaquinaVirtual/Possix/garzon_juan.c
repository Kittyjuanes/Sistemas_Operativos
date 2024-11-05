/*************************************************************************
* Pontificia Universidad Javeriana
* Autor: Juan Esteban Garzón Tinjaca
* Fecha: 5 de Noviembre de 2024
* Parcial N°3 Parte Practica
**************************************************************************/

/***************************************************************************
Este programa implementa la creacion de 5 hilos, los cuales van a ser
impresos en orden como han sido creados y tiene un bucle interno que cuenta
300 enteros.

El modo de ejecución es el siguiente:

1. Compile el programa con la siguiente linea de comando:
	gcc -o nombre_ejecutable garzon_juan.c
	(en "nombre_ejecutable" puede poner el nombre que desee)

2. Una vez creado el archivo ejecutable, corra el ejecutable con el siguiente comando:
	./nombre_ejecutable

**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t BLOQUEO;

 /*Función cualquiera para ejecutar el thread*/
  static void *funcion(void *argumentos){
	pthread_mutex_lock(&BLOQUEO);
	size_t job = *(size_t *)argumentos;
	int cont = 5*job;
	printf("\n\t Hilo %zu  Iniciado\n", job);
	for(int i = 0; i < 300; i++){
		cont *= (cont+1);
	}
	printf("\n\t Hilo %zu Finalizado \n", job);
	pthread_mutex_unlock(&BLOQUEO);

      return NULL;
  }

  int main(int argc, char *argv[]){
      size_t jobs[5];                 //Vector de trabajos|
      pthread_t hilos[5];             //Vector de hilos
      for(int i=0; i<5; i++){
          jobs[i] = i;
          //Creación de hilos y envío de función
          pthread_create(&hilos[i], NULL, &funcion, jobs+i);
      }
	//Llamado a la funcion que ejecuta los threads
      	for(int i=0; i<5; i++)
          	pthread_join(hilos[i], NULL);

	pthread_mutex_destroy(&BLOQUEO);

      return 0;
  }
