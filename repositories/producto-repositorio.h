#ifndef PRODUCTO_REPOSITORIO_H
#define PRODUCTO_REPOSITORIO_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Producto {
    int id;
    char nombre[50];
    char descripcion[100]; 
    float precio;
    char categoria[20]; 
    int stock;
}Producto;

// Prototipos de funciones para Productos

int guardarProductos(const char* nombreArchivo, Producto* productos, int numProductos);
Producto* cargarProductos(const char* nombreArchivo, int* numProductos);
Producto buscarProductoPorId(const char* nombreArchivo, int id);
int agregarProducto(const char* nombreArchivo, Producto nuevoProducto);
int modificarProducto(const char* nombreArchivo, int id, Producto nuevoProducto);
int eliminarProducto(const char* nombreArchivo, int id);
Producto* buscarProductosPorCategoria(const char* nombreArchivo, const char* categoria); // Busca todos los productos de una categoría

#endif