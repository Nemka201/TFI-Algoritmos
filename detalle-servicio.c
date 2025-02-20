#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "detalle-servicio.h"

// Función para mostrar el menú
void mostrarMenuDetalle()
{
    printf("\n-------------------------------------\n");
    printf("\n--- Menu de Ventas ---\n");
    printf("1. Cargar venta a pedido\n");
    printf("2. Mostrar total pedido\n");
    printf("3. Mostrar detalles de un pedido\n");
    printf("4. Volver\n");

    printf("Seleccione una opcion: ");
}

// Nueva función para agregar un detalle de pedido con entrada manual
int agregarDetallePedidoService()
{
    int pedidoId, productoId, cantidad;

    // Pedir ID del pedido

    if (mostrarPedidosConTotales())
    {
        pedidoId = validarNum("Ingrese el ID del pedido: ");
        Pedido pedido = buscarPedidoPorId(FILE_PEDIDOS, pedidoId);
        if (pedido.id == -1)
        {
            printf("Pedido no encontrado.\n");
            return 0;
        }
        mostrarTodosLosProductos();
        // Pedir ID del producto
        productoId = validarNum("Ingrese el ID del producto: ");

        Producto producto = buscarProductoPorId(FILE_PRODUCTOS, productoId);
        if (producto.id == -1)
        {
            printf("Producto no encontrado.\n");
            return 0;
        }

        // Pedir cantidad del producto
        cantidad = validarNum("Ingrese la cantidad: ");

        if (cantidad <= 0)
        {
            printf("La cantidad debe ser mayor a 0.\n");
            return 0;
        }

        // Obtener el último ID registrado y calcular el nuevo
        int ultimoId = obtenerUltimoIdDetallePedido(FILE_DETALLES);
        int nuevoId = (ultimoId == -1) ? 0 : ultimoId + 1;

        DetallePedido nuevoDetalle;
        nuevoDetalle.id = nuevoId;
        nuevoDetalle.pedido = pedido;
        nuevoDetalle.producto = producto;
        nuevoDetalle.cantidad = cantidad;
        nuevoDetalle.subTotalProducto = cantidad * producto.precio;

        // Cargar detalles existentes
        int numDetalles;
        DetallePedido *detalles = cargarDetallesPedidos(FILE_DETALLES, &numDetalles);

        // Agregar el nuevo detalle
        DetallePedido *nuevosDetalles = (DetallePedido *)realloc(detalles, (numDetalles + 1) * sizeof(DetallePedido));
        if (!nuevosDetalles)
        {
            free(detalles);
            return 0;
        }

        nuevosDetalles[numDetalles] = nuevoDetalle;
        numDetalles++;

        // Guardar en el archivo
        if (!guardarDetallesPedidos(FILE_DETALLES, nuevosDetalles, numDetalles))
        {
            free(nuevosDetalles);
            return 0;
        }

        free(nuevosDetalles);
        printf("\n-------------------------------------------------\n");
        printf("Detalle de pedido agregado correctamente.\n");
        printf("==> ID: %d, Pedido ID: %d, Producto: %s, Cantidad: %d, Subtotal: %.2f\n",
               nuevoDetalle.id, nuevoDetalle.pedido.id, nuevoDetalle.producto.nombre, nuevoDetalle.cantidad, nuevoDetalle.subTotalProducto);

        return 1;
    }
}

// Función para mostrar los detalles de un pedido
void mostrarDetallesPedido()
{
    //mostrarPedidosConTotales(); // Muestra los pedidos con totales antes de pedir el ID

    if (mostrarPedidosConTotales())
    {
        int pedidoId;
        pedidoId = validarNum("\nIngrese el ID del Pedido: ");
        int numResultados;
        DetallePedido *detalles = buscarDetallesPorPedido(FILE_DETALLES, pedidoId, &numResultados);

        if (!detalles || numResultados == 0)
        {
            printf("No se encontraron detalles para el pedido ID: %d\n", pedidoId);
            return;
        }

        printf("\nDetalles del Pedido ID: %d\n", pedidoId);
        printf("-------------------------------------------------\n");
        double total = 0.0;

        for (int i = 0; i < numResultados; i++)
        {
            printf("Producto: %s\n", detalles[i].producto.nombre);
            printf("Cantidad: %d\n", detalles[i].cantidad);
            printf("Precio Unitario: %.2f\n", detalles[i].producto.precio);
            printf("Subtotal: %.2f\n", detalles[i].subTotalProducto);
            printf("-------------------------------------------------\n");
            total += detalles[i].subTotalProducto;
        }

        printf("Total a pagar: %.2f\n", total);
        system("pause");
        free(detalles);
    }
}

void mostrarTodosLosDetallesPedidos()
{
    int numDetalles;
    DetallePedido *detalles = cargarDetallesPedidos(FILE_DETALLES, &numDetalles);

    if (!detalles || numDetalles == 0)
    {
        printf("No hay detalles de pedidos registrados.\n");
        return;
    }

    printf("\nTodos los detalles de los pedidos:\n");
    printf("\n%-10s %-20s %-10s %-15s\n", "ID Pedido", "Producto", "Cantidad", "Subtotal");
    printf("-------------------------------------------------\n");

    for (int i = 0; i < numDetalles; i++) {
        printf("%-10d %-20s %-10d %-15.2lf\n", 
               detalles[i].pedido.id, 
               detalles[i].producto.nombre, 
               detalles[i].cantidad, 
               detalles[i].subTotalProducto);
    }

    free(detalles);
}
