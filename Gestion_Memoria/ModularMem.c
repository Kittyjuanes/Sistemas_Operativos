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
#include <stdio.h>

#include "biblioteca.h"


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