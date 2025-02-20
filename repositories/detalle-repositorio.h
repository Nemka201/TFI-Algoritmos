#ifndef DETALLE_REPOSITORIO_H
#define DETALLE_REPOSITORIO_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pedido-repositorio.h"
#include "producto-repositorio.h"

typedef struct DetallePedido {
    int id;
    struct Pedido pedido; 
    struct Producto producto; 
    int cantidad;
    double subTotalProducto;
}DetallePedido;

// Prototipos de funciones para Detalle Pedido
int guardarDetallesPedidos(const char* nombreArchivo, DetallePedido* detalles, int numDetalles);
DetallePedido* cargarDetallesPedidos(const char* nombreArchivo, int* numDetalles);
DetallePedido* buscarDetallesPorPedido(const char* nombreArchivo, int idPedido, int *numResultados);
int agregarDetallePedido(const char* nombreArchivo, DetallePedido nuevoDetalle);
int modificarDetallePedido(const char* nombreArchivo, int id, DetallePedido nuevoDetalle);
int eliminarDetallePedido(const char* nombreArchivo, int id);
Pedido buscarPedidoPorId(const char *nombreArchivo, int id);
int obtenerUltimoIdDetallePedido(const char *nombreArchivo);

#endif