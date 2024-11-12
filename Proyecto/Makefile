# Variables
CC = gcc
CFLAGS = -Wall -g

# Target principal
all: publicador suscriptor sistema

# Reglas de compilación
publicador: Publicador.c
	$(CC) $(CFLAGS) -o publicador Publicador.c

suscriptor: Suscriptor3.c 
	$(CC) $(CFLAGS) -o suscriptor Suscriptor3.c

sistema: Sistema3_Comunicacion.c 
	$(CC) $(CFLAGS) -o sistema Sistema3_Comunicacion.c

# Limpieza
clean:
	rm -f publicador suscriptor sistema
