#ifndef DETALLE_SERVICIO_H
#define DETALLE_SERVICIO_H

#include "repositories.h"
#include "detalle-servicio.h"
#include "pedido-servicio.h"
#include "producto-servicio.h"

// Funciones prototipo
void mostrarMenuDetalle();
int obtenerUltimoIdDetallePedido(const char *nombreArchivo);
int agregarDetallePedidoService();
// double calcularTotalPedido(int pedidoId);
void mostrarDetallesPedido(int pedidoId);

#endif