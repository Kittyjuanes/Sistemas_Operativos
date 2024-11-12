#!/usr/bin/perl
#**************************************************************
#           Pontificia Universidad Javeriana
#     Autor: J. Corredor
#     Fecha: Febrero 2024
#     Materia: Sistemas Operativos
#     Tema: Taller de Evaluación de Rendimiento
#     Fichero: script automatización ejecución por lotes 
#****************************************************************/

# Obtiene el directorio de trabajo actual y elimina el salto de línea
$Path = `pwd`;
chomp($Path);

# Nombre del ejecutable que se usará
$Nombre_Ejecutable = "clasico";

# Rubricas para la experimentacion
@Size_Matriz = ("800","1000","1200","1400","1600");
@Num_Hilos = (1,2,4,8);
$Repeticiones = 30;

# Se ejecuta los experimentos
foreach $size (@Size_Matriz){
    foreach $hilo (@Num_Hilos) {
        # Nombre del archivo de salida para los resultados de esta configuración
        $file = "$Path/$Nombre_Ejecutable-".$size."-Hilos-".$hilo.".dat";
        
        for ($i=0; $i<$Repeticiones; $i++) {
            # Ejecuta el comando y redirige la salida al archivo correspondiente
            system("$Path/$Nombre_Ejecutable $size $hilo  >> $file");
            
            # Imprime en pantalla el comando que se ejecuta (para seguimiento)
            #printf("$Path/$Nombre_Ejecutable $size $hilo \n");
        }
        
        # Cierra el archivo
        close($file);

        # Incrementa un contador
        $p = $p + 1;
    }
}

