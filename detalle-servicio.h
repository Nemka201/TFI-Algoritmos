#ifndef DETALLE_SERVICIO_H
#define DETALLE_SERVICIO_H

#include "repositories.h"
#include "detalle-servicio.h"
#include "pedido-servicio.h"
#include "producto-servicio.h"

// Funciones prototipo
int obtenerUltimoIdDetallePedido(const char *nombreArchivo);
DetallePedido *cargarDetallesPedidos(const char *nombreArchivo, int *numDetalles);
int guardarDetallesPedidos(const char *nombreArchivo, DetallePedido *detalles, int numDetalles);
int agregarDetallePedido(const char *nombreArchivo);
Pedido buscarPedidoPorId(const char *nombreArchivo, int id);
DetallePedido *buscarDetallePedido(const char *nombreArchivo, int id);
int modificarDetallePedido(const char *nombreArchivo, int id, int nuevaCantidad, double nuevoPrecio);
int eliminarDetallePedido(const char *nombreArchivo, int id);
void listarDetallesPedidos(const char *nombreArchivo);


#endif