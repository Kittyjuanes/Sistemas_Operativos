/*************************************************************
Autor: Juan Esteban Garzon Tinjaca
Materia: Ssitemas Operativos
Pontificia Universidad Javeriana
tema:  GestiÃ³n de memoria, punteros y memoria dinÃ¡mica

        Gestion de memoria(dinÃ¡mica, estÃ¡tica): Manipulacion de memoria
        segun necesite la aplicacion, por parte del usuario
        se presenta la memoria estÃ¡tica que solo se dispone en tiempo de compilaciÃ³n. Variables
        globales, vectores fijos, etc. No es necesario liberar memoria y siempre es de tamaÃ±o fijo.
        Se presenta la memoria dinÃ¡mica que el usuario en funciÃ³n del problema puede cambiar, modificar
        o liberar en tiempo de ejecuciÃ³n. Esto se realiza mediante funciones especÃ­ficas proporcionadas
        por el sistema operativo. A diferencia de la memoria estÃ¡tica,
        la memoria dinÃ¡mica permite a la aplicaciÃ³n solicitar y liberar memoria en tiempo real, adaptÃ¡ndose
        a las necesidades cambiantes del programa.

        punteros: son variables auxiliares con herramientas a disposiciÃ³n del usuario para de forma ligera seÃ±alar              espacio de memoria o contenedores (estructura de datos.etc) y obtener de forma precisa la direcciÃ³n de memoria
        Ãºtiles cuando se queiere que el programa sea ligero al administradr el uso de memoria.

        compilaciÃ³n modular: representa  crear una biblioteca de funciones con su itnerfaz correspondiente. de forma            que puede la biblioteca a disposicion de otros porgramas o usuarios. Adiciona, se crea un fichero que se llama          makefile cuyo proÃ³sito es el de automatizar la compilaciÃ³n

*****************************************************************/





#define CAPACIDAD_INICIAL 4
#include <stdlib.h>  
#include <stdio.h>  //Falta de librerias dentro del cÃ³digo


/*CreaciÃ³n Estructura de Datos para el Vector*/
typedef struct vectorDinamico{
        int capacidad;
        int totalElementos;
        void **elementos;
} vectorDinamico;

//Asigna la capacidad inicial del vector dinÃ¡mico
void vectorInicio(vectorDinamico *V){
        V->capacidad = CAPACIDAD_INICIAL;
        V->totalElementos = 0;
        V->elementos = malloc(sizeof(void *) * V->capacidad); 
}

//Cantidad total de elementos del vector dinÃ¡mico
int totalVector(vectorDinamico *V){
        return V->totalElementos;
}

//TamaÃ±o de la capacidad del vector dinÃ¡mico
static void resizeVector(vectorDinamico *V, int capacidad){
        printf("\nRedimensiÃ³n: %d a %d \n\n\n", V->capacidad, capacidad);

        void **elementos = realloc(V->elementos, sizeof(void *) * capacidad);
        if(elementos){
                V->elementos = elementos;
                V->capacidad = capacidad;
        }
}

//AÃ±adir elementos al vector dinÃ¡mico
void addVector(vectorDinamico *V, void *elemento){
        if(V->capacidad == V->totalElementos)
                resizeVector(V, V->capacidad*2);
        V->elementos[V->totalElementos++] = elemento;
}

//LibearciÃ³n de memoria del vector dinÃ¡mico
void freeVector(vectorDinamico *V){
        free(V->elementos);
}

//Get del vector dinÃ¡mico
void *getVector(vectorDinamico *V, int indice){
        if(indice >= 0 && indice < V->totalElementos)
                return V->elementos[indice];
        return NULL;
}

//Setear el valor de un elemento del vector dinÃ¡mico
void setVector(vectorDinamico *V, int indice, void *elemento){
        if(indice >= 0 && indice < V->totalElementos)
                V->elementos[indice]=elemento;
}

//Eliminar el vector dinÃ¡mico
void borrarVector(vectorDinamico *V, int indice){
        if(indice < 0 || indice >= V->totalElementos)
                return; 

        V->elementos[indice] = NULL;

        for(int i=indice; i<V->totalElementos-1; i++){
                V->elementos[i] = V->elementos[i+1];
                V->elementos[i+1] = NULL; 
        }
        V->totalElementos--;
        if(V->totalElementos>0 && V->totalElementos == V->capacidad/4)
                resizeVector(V, V->capacidad/2);
}


//FunciÃ³n para imprimir el vector dinÃ¡mico
int main(){
        vectorDinamico editor;
        vectorInicio(&editor);

        addVector(&editor, "Hola");
        addVector(&editor, " Profesional");
        addVector(&editor, " en");
        addVector(&editor, " FormaciÃ³n");
        addVector(&editor, " de");
        addVector(&editor, " IngenierÃ­a\n\n");

        
       for (int i = 0; i < totalVector(&editor); i++)
       printf("%s", (char *) getVector(&editor, i));

        borrarVector(&editor, 4);
        borrarVector(&editor, 3);
        borrarVector(&editor, 2);
        borrarVector(&editor, 1);


  
        setVector(&editor, 1, "Buenos");
        addVector(&editor, "DÃ­as\n\n");


        for (int i = 0; i < totalVector(&editor); i++)
        printf("%s ", (char *) getVector(&editor, i));

        freeVector(&editor);  //Liberar Memoria
        return 0;
}