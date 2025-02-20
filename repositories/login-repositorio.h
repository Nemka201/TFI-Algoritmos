#ifndef LOGIN_REPOSITORIO_H
#define LOGIN_REPOSITORIO_H
#define ARCHIVO_USUARIOS "Usuarios.dat"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Usuario{
    char username[10];
    char password[10];
    char nombreCompleto[60];
} Usuario;

// Prototipos para manejar persistencia de usuarios

int cargarUsuarios(Usuario usuarios[], int maxUsuarios);
void guardarUsuarios(Usuario usuarios[], int cantidadUsuarios);

#endif