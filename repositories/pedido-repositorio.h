#ifndef PEDIDO_REPOSITORIO_H
#define PEDIDO_REPOSITORIO_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mesa-repositorio.h"

typedef struct Pedido {
    int id;
    struct Mesa mesa; 
    char fechaHora[30]; 
    char estado[20]; 
}Pedido;

// Prototipos de funciones para Pedidos

int obtenerUltimoIdPedido(const char *nombreArchivo);
int guardarPedidos(const char* nombreArchivo, Pedido* pedidos, int numPedidos);
Pedido* cargarPedidos(const char* nombreArchivo, int* numPedidos);
Pedido buscarPedidoPorId(const char* nombreArchivo, int id);
int agregarPedido(const char* nombreArchivo, Pedido nuevoPedido);
int modificarPedido(const char* nombreArchivo, int id, Pedido nuevoPedido);
int eliminarPedido(const char* nombreArchivo, int id);
Pedido* buscarPedidosPorMesa(const char* nombreArchivo, int idMesa, int* numPedidos); // Busca todos los pedidos de una mesa
float calcularTotalPedido(const char* nombreArchivo, int idPedido); // Calcula el total de un pedido
void crearArchivoPedidos();

#endif