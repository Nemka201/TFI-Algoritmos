#ifndef DETALLE_SERVICIO_H
#define DETALLE_SERVICIO_H
#define FILE_DETALLES "detalles.bin"
#define FILE_PRODUCTOS "productos.bin"
#define FILE_PEDIDOS "pedido.bin"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../repositories/detalle-repositorio.h"
#include "detalle-servicio.h"
#include "pedido-servicio.h"
#include "producto-servicio.h"
#include "../utilities.h"

// Funciones prototipo
void mostrarMenuDetalle();
int obtenerUltimoIdDetallePedido(const char *nombreArchivo);
int agregarDetallePedidoService();
void mostrarDetallesPedido();
void mostrarTodosLosDetallesPedidos();

#endif