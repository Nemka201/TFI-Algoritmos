#ifndef MESA_SERVICIO_H
#define MESA_SERVICIO_H

#include "repositories.h"

// Funciones prototipo

int reservarMesa(int idMesa);
int liberarMesa(int idMesa);
void mostrarDisponibilidadMesas();
void mostrarMenuMesas();

// Funciones para acciones del menu

void accionMostrarDisponibilidad();
void accionReservarMesa();
void accionLiberarMesa();
void accionMostrarDisponibilidad();
void accionReservarMesa();
void accionLiberarMesa();
void accionAgregarMesa();
void accionBuscarMesaPorId();

#endif