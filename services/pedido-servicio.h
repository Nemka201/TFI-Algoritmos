#ifndef PEDIDO_SERVICIO_H
#define PEDIDO_SERVICIO_H
#define FILE_PEDIDO "pedido.bin"
#define FILE_MESAS "mesas.bin"
#define FILE_DETALLES "detalles.bin"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "../repositories/pedido-repositorio.h"
#include "../utilities.h"

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