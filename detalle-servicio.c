#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "detalle-servicio.h"

#define FILE_DETALLE_PEDIDO "detalles_pedido.dat"
#define FILE_PRODUCTOS "productos.bin"
#define FILE_PEDIDOS "pedido.bin"

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

// Cargar los detalles de pedido desde el archivo binario
DetallePedido *cargarDetallesPedidos(const char *nombreArchivo, int *numDetalles)
{
    FILE *fp = fopen(nombreArchivo, "rb");
    if (!fp)
    {
        *numDetalles = 0;
        return NULL;
    }

    if (fread(numDetalles, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        *numDetalles = 0;
        return NULL;
    }

    if (*numDetalles <= 0)
    {
        fclose(fp);
        return NULL;
    }

    DetallePedido *detalles = (DetallePedido *)malloc((*numDetalles) * sizeof(DetallePedido));
    if (!detalles)
    {
        fclose(fp);
        return NULL;
    }

    if (fread(detalles, sizeof(DetallePedido), *numDetalles, fp) != (size_t)(*numDetalles))
    {
        free(detalles);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return detalles;
}

// Guardar los detalles de pedido en el archivo binario
int guardarDetallesPedidos(const char *nombreArchivo, DetallePedido *detalles, int numDetalles)
{
    FILE *fp = fopen(nombreArchivo, "wb");
    if (!fp)
    {
        return 0;
    }

    if (fwrite(&numDetalles, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }

    if (fwrite(detalles, sizeof(DetallePedido), numDetalles, fp) != (size_t)numDetalles)
    {
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return 1;
}

// Nueva función para agregar un detalle de pedido con entrada manual
int agregarDetallePedido(const char *nombreArchivo)
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
    int ultimoId = obtenerUltimoIdDetallePedido(nombreArchivo);
    int nuevoId = (ultimoId == -1) ? 0 : ultimoId + 1;

    DetallePedido nuevoDetalle;
    nuevoDetalle.id = nuevoId;
    nuevoDetalle.pedido = pedido;
    nuevoDetalle.producto = producto;
    nuevoDetalle.cantidad = cantidad;
    nuevoDetalle.subTotalProducto = cantidad * producto.precio;

    // Cargar detalles existentes
    int numDetalles;
    DetallePedido *detalles = cargarDetallesPedidos(nombreArchivo, &numDetalles);

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
    if (!guardarDetallesPedidos(nombreArchivo, nuevosDetalles, numDetalles))
    {
        free(nuevosDetalles);
        return 0;
    }

    free(nuevosDetalles);
    printf("Detalle de pedido agregado correctamente.\n");
    return 1;
}

// Función para buscar un pedido por ID
Pedido buscarPedidoPorId(const char *nombreArchivo, int id)
{
    FILE *fp = fopen(nombreArchivo, "rb");
    Pedido pedido;
    pedido.id = -1; // Indicar que no se encontró

    if (!fp)
        return pedido;

    while (fread(&pedido, sizeof(Pedido), 1, fp) == 1)
    {
        if (pedido.id == id)
        {
            fclose(fp);
            return pedido;
        }
    }

    fclose(fp);
    pedido.id = -1;
    return pedido;
}

// Buscar un detalle de pedido por ID
DetallePedido *buscarDetallePedido(const char *nombreArchivo, int id)
{
    int numDetalles;
    DetallePedido *detalles = cargarDetallesPedidos(nombreArchivo, &numDetalles);

    for (int i = 0; i < numDetalles; i++)
    {
        if (detalles[i].id == id)
        {
            DetallePedido *detalleEncontrado = (DetallePedido *)malloc(sizeof(DetallePedido));
            memcpy(detalleEncontrado, &detalles[i], sizeof(DetallePedido));
            free(detalles);
            return detalleEncontrado;
        }
    }

    free(detalles);
    return NULL;
}

// Modificar un detalle de pedido
int modificarDetallePedido(const char *nombreArchivo, int id, int nuevaCantidad, double nuevoPrecio)
{
    int numDetalles;
    DetallePedido *detalles = cargarDetallesPedidos(nombreArchivo, &numDetalles);

    for (int i = 0; i < numDetalles; i++)
    {
        if (detalles[i].id == id)
        {
            detalles[i].cantidad = nuevaCantidad;
            detalles[i].subTotalProducto = nuevaCantidad * nuevoPrecio;
            guardarDetallesPedidos(nombreArchivo, detalles, numDetalles);
            free(detalles);
            return 1;
        }
    }

    free(detalles);
    return 0;
}

// Eliminar un detalle de pedido
int eliminarDetallePedido(const char *nombreArchivo, int id)
{
    int numDetalles;
    DetallePedido *detalles = cargarDetallesPedidos(nombreArchivo, &numDetalles);

    int indiceEliminar = -1;
    for (int i = 0; i < numDetalles; i++)
    {
        if (detalles[i].id == id)
        {
            indiceEliminar = i;
            break;
        }
    }

    if (indiceEliminar == -1)
    {
        free(detalles);
        return 0;
    }

    for (int i = indiceEliminar; i < numDetalles - 1; i++)
    {
        detalles[i] = detalles[i + 1];
    }

    numDetalles--;

    if (!guardarDetallesPedidos(nombreArchivo, detalles, numDetalles))
    {
        free(detalles);
        return 0;
    }

    free(detalles);
    return 1;
}

// Listar todos los detalles de pedido
void listarDetallesPedidos(const char *nombreArchivo)
{
    int numDetalles;
    DetallePedido *detalles = cargarDetallesPedidos(nombreArchivo, &numDetalles);

    if (!detalles || numDetalles == 0)
    {
        printf("No hay detalles de pedido registrados.\n");
        return;
    }

    printf("ID | Pedido ID | Producto ID | Cantidad | Subtotal\n");
    for (int i = 0; i < numDetalles; i++)
    {
        printf("%d | %d | %d | %d | %.2f\n",
               detalles[i].id, detalles[i].pedido.id,
               detalles[i].producto.id, detalles[i].cantidad, detalles[i].subTotalProducto);
    }
    free(detalles);
}