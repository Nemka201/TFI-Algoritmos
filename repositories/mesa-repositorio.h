#ifndef MESA_REPOSITORIO_H
#define MESA_REPOSITORIO_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Mesa {
    int id;
    int capacidad;
    char ubicacion[50];
    bool disponible;
}Mesa;

// Prototipos de funciones para Mesas

void guardarMesas(const char* nombreArchivo, Mesa* mesas, int numMesas);
Mesa* cargarMesas(const char* nombreArchivo, int* numMesas);
Mesa buscarMesaPorId(const char* nombreArchivo, int id);
int agregarMesa(const char* nombreArchivo, Mesa nuevaMesa);
int modificarMesa(const char* nombreArchivo, int id, Mesa nuevaMesa);
int eliminarMesa(const char* nombreArchivo, int id);

#endif