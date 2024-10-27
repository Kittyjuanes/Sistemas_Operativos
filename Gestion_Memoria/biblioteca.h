/*************************************************************
Autor: Juan Esteban Garzon Tinjaca
Materia: Ssitemas Operativos
Pontificia Universidad Javeriana
tema:  Gestión de memoria, punteros y memoria dinámica

        Gestion de memoria(dinámica, estática): Manipulacion de memoria
        segun necesite la aplicacion, por parte del usuario
        se presenta la memoria estática que solo se dispone en tiempo de compilación. Variables
        globales, vectores fijos, etc. No es necesario liberar memoria y siempre es de tamaño fijo.
        Se presenta la memoria dinámica que el usuario en función del problema puede cambiar, modificar
        o liberar en tiempo de ejecución. Esto se realiza mediante funciones específicas proporcionadas
        por el sistema operativo. A diferencia de la memoria estática,
        la memoria dinámica permite a la aplicación solicitar y liberar memoria en tiempo real, adaptándose
        a las necesidades cambiantes del programa.

        punteros: son variables auxiliares con herramientas a disposición del usuario para de forma ligera señalar              espacio de memoria o contenedores (estructura de datos.etc) y obtener de forma precisa la dirección de memoria
        útiles cuando se queiere que el programa sea ligero al administradr el uso de memoria.

        compilación modular: representa  crear una biblioteca de funciones con su itnerfaz correspondiente. de forma            que puede la biblioteca a disposicion de otros porgramas o usuarios. Adiciona, se crea un fichero que se llama          makefile cuyo proósito es el de automatizar la compilación
        
*****************************************************************/
/********************************************************************
*                INTERFAZ DE FUNCIONES DE LA BIBLIOTECA       *
*********************************************************************/

#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#define CAPACIDAD_INICIAL 4

typedef struct vectorDinamico{
        int capacidad;
        int totalElementos;
        void **elementos;
} vectorDinamico;

void vectorInicio(vectorDinamico *V);

int totalVector(vectorDinamico *V);

static void resizeVector(vectorDinamico *V, int capacidad);

void addVector(vectorDinamico *V, void *elemento);

void freeVector(vectorDinamico *V);

void *getVector(vectorDinamico *V, int indice);

void setVector(vectorDinamico *V, int indice, void *elemento);

void borrarVector(vectorDinamico *V, int indice);

#endif
