#ifndef PEDIDO_SERVICIO_H
#define PEDIDO_SERVICIO_H
#define FILE_PEDIDO "pedido.bin"
#define FILE_MESAS "mesas.bin"
#define FILE_DETALLES "detalles.bin"
#include "repositories.h"

// Funciones prototipo

void mostrarMenuPedido();
void agregarPedidoMenu();
void buscarPedidoMenu();
void modificarPedidoMenu();
void eliminarPedidoMenu();
void buscarPedidosPorMesaMenu();
void calcularTotalPedidoMenu();
bool mostrarPedidosConTotales();

#endif