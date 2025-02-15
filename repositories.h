#ifndef REPOSITORIES_H
#define REPOSITORIES_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

    // STRUCTS //
typedef struct Mesa {
    int id;
    int capacidad;
    char ubicacion[50];
    bool disponible;
}Mesa;

typedef struct Pedido {
    int id;
    struct Mesa mesa; 
    char fechaHora[30]; 
    char estado[20]; 
}Pedido;

typedef struct Producto {
    int id;
    char nombre[50];
    char descripcion[100]; 
    float precio;
    char categoria[20]; 
    int stock;
}Producto;

typedef struct DetallePedido {
    int id;
    struct Pedido pedido; 
    struct Producto producto; 
    int cantidad;
    double subTotalProducto;
}DetallePedido;

// Prototipos de funciones para Mesas

void guardarMesas(const char* nombreArchivo, Mesa* mesas, int numMesas);
Mesa* cargarMesas(const char* nombreArchivo, int* numMesas);
Mesa buscarMesaPorId(const char* nombreArchivo, int id);
int agregarMesa(const char* nombreArchivo, Mesa nuevaMesa);
int modificarMesa(const char* nombreArchivo, int id, Mesa nuevaMesa);
int eliminarMesa(const char* nombreArchivo, int id);

// Prototipos de funciones para Pedidos

void guardarPedidos(const char* nombreArchivo, Pedido* pedidos, int numPedidos);
Pedido* cargarPedidos(const char* nombreArchivo, int* numPedidos);
Pedido buscarPedidoPorId(const char* nombreArchivo, int id);
int agregarPedido(const char* nombreArchivo, Pedido nuevoPedido);
int modificarPedido(const char* nombreArchivo, int id, Pedido nuevoPedido);
int eliminarPedido(const char* nombreArchivo, int id);
Pedido* buscarPedidosPorMesa(const char* nombreArchivo, int idMesa, int* numPedidos); // Busca todos los pedidos de una mesa
float calcularTotalPedido(const char* nombreArchivo, int idPedido, int* numPedidos); // Calcula el total de un pedido

// Prototipos de funciones para Productos

int guardarProductos(const char* nombreArchivo, Producto* productos, int numProductos);
Producto* cargarProductos(const char* nombreArchivo, int* numProductos);
Producto buscarProductoPorId(const char* nombreArchivo, int id);
int agregarProducto(const char* nombreArchivo, Producto nuevoProducto);
int modificarProducto(const char* nombreArchivo, int id, Producto nuevoProducto);
int eliminarProducto(const char* nombreArchivo, int id);
Producto* buscarProductosPorCategoria(const char* nombreArchivo, const char* categoria); // Busca todos los productos de una categoría

// Prototipos de funciones para Detalles de Pedidos

void guardarDetallesPedidos(const char* nombreArchivo, DetallePedido* detalles, int numDetalles);
DetallePedido* cargarDetallesPedidos(const char* nombreArchivo, int* numDetalles);
DetallePedido* buscarDetallesPorPedido(const char* nombreArchivo, int idPedido);
int agregarDetallePedido(const char* nombreArchivo, DetallePedido nuevoDetalle);
int modificarDetallePedido(const char* nombreArchivo, int id, DetallePedido nuevoDetalle);
int eliminarDetallePedido(const char* nombreArchivo, int id);

// Funciones auxiliares

int encontrarIndiceLibre(const char* nombreArchivo, int tamEstructura); // Encuentra el siguiente ID disponible
void imprimirMesa(Mesa mesa); // Imprime los datos de una mesa en pantalla

#endif