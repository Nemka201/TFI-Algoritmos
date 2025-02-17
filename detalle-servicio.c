#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "detalle-servicio.h"

#define FILE_DETALLE_PEDIDO "detalles_pedido.dat"
#define FILE_PRODUCTOS "productos.bin"
#define FILE_PEDIDOS "pedido.bin"

// Función para mostrar el menú
void mostrarMenuDetalle()
{
    printf("\n-------------------------------------\n");
    printf("\n--- Menu de Gestion de Pedidos ---\n");
    printf("1. Cargar venta a pedido\n");
    printf("2. Mostrar total pedido\n");
    printf("3. Mostrar detalles de un pedido\n");
    printf("4. Volver\n");

    printf("Seleccione una opcion: ");
}

// Obtener el último ID registrado para asignar uno nuevo
int obtenerUltimoIdDetallePedido(const char *nombreArchivo)
{
    FILE *fp = fopen(nombreArchivo, "rb");
    if (!fp)
    {
        return -1; // Si el archivo no existe, comenzamos desde 0
    }

    int numDetalles;
    if (fread(&numDetalles, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return -1;
    }

    if (numDetalles <= 0)
    {
        fclose(fp);
        return -1;
    }

    fseek(fp, sizeof(int) + (numDetalles - 1) * sizeof(DetallePedido), SEEK_SET);

    DetallePedido ultimoDetalle;
    if (fread(&ultimoDetalle, sizeof(DetallePedido), 1, fp) != 1)
    {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return ultimoDetalle.id;
}

// Nueva función para agregar un detalle de pedido con entrada manual
int agregarDetallePedidoService()
{
    int pedidoId, productoId, cantidad;

    // Pedir ID del pedido
    printf("Ingrese el ID del pedido: ");
    scanf("%d", &pedidoId);

    Pedido pedido = buscarPedidoPorId(FILE_PEDIDOS, pedidoId);
    if (pedido.id == -1)
    {
        printf("Pedido no encontrado.\n");
        return 0;
    }

    // Pedir ID del producto
    printf("Ingrese el ID del producto: ");
    scanf("%d", &productoId);

    Producto producto = buscarProductoPorId(FILE_PRODUCTOS, productoId);
    if (producto.id == -1)
    {
        printf("Producto no encontrado.\n");
        return 0;
    }

    // Pedir cantidad del producto
    printf("Ingrese la cantidad: ");
    scanf("%d", &cantidad);

    if (cantidad <= 0)
    {
        printf("La cantidad debe ser mayor a 0.\n");
        return 0;
    }

    // Obtener el último ID registrado y calcular el nuevo
    int ultimoId = obtenerUltimoIdDetallePedido(FILE_DETALLE_PEDIDO);
    int nuevoId = (ultimoId == -1) ? 0 : ultimoId + 1;

    DetallePedido nuevoDetalle;
    nuevoDetalle.id = nuevoId;
    nuevoDetalle.pedido = pedido;
    nuevoDetalle.producto = producto;
    nuevoDetalle.cantidad = cantidad;
    nuevoDetalle.subTotalProducto = cantidad * producto.precio;

    // Cargar detalles existentes
    int numDetalles;
    DetallePedido *detalles = cargarDetallesPedidos(FILE_DETALLE_PEDIDO, &numDetalles);

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
    if (!guardarDetallesPedidos(FILE_DETALLE_PEDIDO, nuevosDetalles, numDetalles))
    {
        free(nuevosDetalles);
        return 0;
    }

    free(nuevosDetalles);
    printf("Detalle de pedido agregado correctamente.\n");
    return 1;
}
// Función para mostrar los detalles de un pedido
void mostrarDetallesPedido(int pedidoId)
{
    int numDetalles;
    DetallePedido *detalles = cargarDetallesPedidos(FILE_DETALLE_PEDIDO, &numDetalles);

    if (!detalles || numDetalles == 0)
    {
        printf("No hay detalles de pedidos registrados.\n");
        return;
    }

    printf("\nDetalles del Pedido ID: %d\n", pedidoId);
    printf("-------------------------------------------------\n");
    double total = 0.0;

    for (int i = 0; i < numDetalles; i++)
    {
        if (detalles[i].pedido.id == pedidoId)
        {
            printf("Producto: %s\n", detalles[i].producto.nombre);
            printf("Cantidad: %d\n", detalles[i].cantidad);
            printf("Precio Unitario: %.2f\n", detalles[i].producto.precio);
            printf("Subtotal: %.2f\n", detalles[i].subTotalProducto);
            printf("-------------------------------------------------\n");
            total += detalles[i].subTotalProducto;
        }
    }

    printf("Total a pagar: %.2f\n", total);
    free(detalles);
}

// // Función para calcular el total de un pedido
// double calcularTotalPedido(int pedidoId)
// {
//     int numDetalles;
//     DetallePedido *detalles = cargarDetallesPedidos(FILE_DETALLE_PEDIDO, &numDetalles);

//     if (!detalles || numDetalles == 0)
//     {
//         return 0.0;
//     }

//     double total = 0.0;
//     for (int i = 0; i < numDetalles; i++)
//     {
//         if (detalles[i].pedido.id == pedidoId)
//         {
//             total += detalles[i].subTotalProducto;
//         }
//     }

//     free(detalles);
//     return total;
// }