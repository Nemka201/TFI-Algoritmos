#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mesa-repositorio.h"

// Funciones auxiliares
int validarNum(const char *mensaje);
int encontrarIndiceLibre(const char* nombreArchivo, int tamEstructura); // Encuentra el siguiente ID disponible
void imprimirMesa(Mesa mesa); // Imprime los datos de una mesa en pantalla
void esperarTecla();
#endif