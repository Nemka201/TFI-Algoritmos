#include "pedido-servicio.h"
#include "mesa-servicio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

void mostrarMenuPedido()
{
    crearArch();
    printf("\n--- Menu de Gestion de Pedidos ---\n");
    printf("1. Guardar pedido\n");
    printf("2. Buscar pedido\n");
    printf("3. Editar pedido\n");
    printf("4. Eliminar pedido\n");
    printf("5. Buscar pedidios por mesa\n");
    printf("6. Mostrar todos los pedidos\n");
    printf("7. Volver\n");
    printf("Seleccione una opcion: ");
}

void crearArch()
{
    crearArchivoPedidos();
}

int obtenerUltimoIdPedido(const char *nombreArchivo)
{
    FILE *fp = fopen(nombreArchivo, "rb");
    if (!fp)
    {
        return 0; // Si el archivo no existe, empezamos desde 0
    }

    int numPedidos;
    if (fread(&numPedidos, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return 0; // Si no se puede leer el número de pedidos, empezamos desde 0
    }

    if (numPedidos <= 0)
    {
        fclose(fp);
        return 0; // No hay pedidos registrados
    }

    // Ir al último pedido
    fseek(fp, sizeof(int) + (numPedidos - 1) * sizeof(Pedido), SEEK_SET);

    Pedido ultimoPedido;
    if (fread(&ultimoPedido, sizeof(Pedido), 1, fp) != 1)
    {
        fclose(fp);
        return 0; // Si hay un error, asumimos que no hay pedidos válidos
    }

    fclose(fp);
    return ultimoPedido.id;
}

void agregarPedidoMenu()
{
    Pedido nuevoPedido;
    getchar();

    // Obtener el último ID registrado
    int ultimoId = obtenerUltimoIdPedido(FILE_PEDIDO);
    nuevoPedido.id = (ultimoId == -1) ? 0 : ultimoId + 1;

    printf("ID: %d\n", nuevoPedido.id);

    // Solicitar ID de la mesa asociada al pedido
    printf("Ingrese el ID de la mesa: ");
    int idMesa;
    scanf("%d", &idMesa);

    // Buscar la mesa por ID en el archivo binario
    Mesa mesaEncontrada = buscarMesaPorId(FILE_MESAS, idMesa);

    if (mesaEncontrada.id == 0)
    {
        printf("No se encontro ninguna mesa con el ID %d. Pedido no agregado.\n", idMesa);
        return;
    }

    // reservarMesa(mesaEncontrada.id);
    nuevoPedido.mesa = mesaEncontrada;

    getchar();

    // Obtener la fecha y hora actual
    time_t now = time(0);
    struct tm *tlocal = localtime(&now);
    char output[sizeof "YYYY-MM-DD HH:MM:SS"];
    strftime(output, sizeof(output), "%Y-%m-%d %H:%M:%S", tlocal);
    strcpy(nuevoPedido.fechaHora, output);

    // Obtener el estado del pedido desde el usuario
    int estado;
    do
    {
        printf("Ingrese el estado del pedido:\n");
        printf("1. Pendiente\n");
        printf("2. En proceso\n");
        printf("3. Completado\n");
        scanf("%d", &estado);
    } while (estado < 1 || estado > 3);

    switch (estado)
    {
    case 1:
        strcpy(nuevoPedido.estado, "Pendiente");
        break;
    case 2:
        strcpy(nuevoPedido.estado, "En proceso");
        break;
    case 3:
        strcpy(nuevoPedido.estado, "Completado");
        break;
    default:
        printf("Opción inválida.\n");
        break;
    }

    // Guardar el pedido en el archivo binario
    if (agregarPedido(FILE_PEDIDO, nuevoPedido))
    {
        printf("Pedido agregado correctamente con ID %d.\n", nuevoPedido.id);
    }
    else
    {
        printf("Error al agregar el pedido.\n");
    }
}

void buscarPedidoMenu()
{
    int idPedido;
    printf("Ingrese el ID del pedido a buscar: ");
    scanf("%d", &idPedido);
    Pedido pedidoEncontrado = buscarPedidoPorId(FILE_PEDIDO, idPedido);

    if (pedidoEncontrado.id != 0)
    {
        printf("\n-------------------------------------\n");
        printf("Pedido encontrado:\n");
        printf("ID Pedido: %d\n", pedidoEncontrado.id);
        printf("ID Mesa: %d\n", pedidoEncontrado.mesa.id);
        printf("Capacidad: %d\n", pedidoEncontrado.mesa.capacidad);
        printf("Ubicacion: %s\n", pedidoEncontrado.mesa.ubicacion);
        printf("Disponibilidad: %s\n", pedidoEncontrado.estado);
        printf("Fecha y hora: %s\n", pedidoEncontrado.fechaHora);
        printf("-------------------------------------\n");
    }
    else
    {
        printf("No se encontro ningun pedido con el ID %d.\n", idPedido);
    }
}

void modificarPedidoMenu()
{
    int id, idMesaAux;
    Pedido pedidoActual, nuevoPedido;

    printf("Ingrese el ID del pedido a modificar: ");
    scanf("%d", &id);

    pedidoActual = buscarPedidoPorId(FILE_PEDIDO, id);
    if (pedidoActual.id == 0)
    {
        printf("Pedido no encontrado\n");
        return;
    }
    while (getchar() != '\n')
        ; // Limpiar buffer después de scanf
    // Copiar los datos del pedido actual al nuevo producto
    idMesaAux = pedidoActual.mesa.id;
    nuevoPedido = pedidoActual;
    printf("Ingrese los nuevos datos del pedido:\n");
    printf("Estado: ");
    fgets(nuevoPedido.estado, sizeof(nuevoPedido.estado), stdin);
    // Eliminar el salto de línea al final del Estado, si existe
    nuevoPedido.estado[strcspn(nuevoPedido.estado, "\n")] = '\0';

    printf("ID de la mesa: ");
    scanf("%d", &nuevoPedido.mesa.id);
    if (nuevoPedido.mesa.id != idMesaAux)
    {
        liberarMesa(idMesaAux);
    }

    // Solo si alguno de los datos fue modificado, actualizar el pedido
    if (strcmp(nuevoPedido.estado, pedidoActual.estado) != 0 ||
        nuevoPedido.mesa.id != pedidoActual.mesa.id)
    {
        if (!modificarPedido(FILE_PEDIDO, id, nuevoPedido))
        {
            printf("Error al modificar el pedido\n");
        }
        else
        {
            printf("Pedido modificado correctamente\n");
        }
    }
    else
    {
        printf("No se realizaron cambios en el pedido\n");
    }
}

void eliminarPedidoMenu()
{
    int id;

    printf("Ingrese el ID del pedido a eliminar: ");
    scanf("%d", &id);

    if (eliminarPedido(FILE_PEDIDO, id))
    {
        printf("Pedido eliminado correctamente.\n");
    }
    else
    {
        printf("Error: No se pudo eliminar el pedido.\n");
    }
}

void buscarPedidosPorMesaMenu()
{
    int idMesa;
    printf("Ingrese el ID de la mesa a consultar: ");
    scanf("%d", &idMesa);

    int numPedidos;
    Pedido *pedidosMesa = buscarPedidosPorMesa(FILE_PEDIDO, idMesa, &numPedidos);

    if (pedidosMesa == NULL)
    {
        printf("No se encontraron pedidos para la mesa %d.\n", idMesa);
        return;
    }

    printf("Pedidos encontrados para la mesa %d:\n", idMesa);
    for (int i = 0; i < numPedidos; i++)
    {
        printf("\n-------------------------------------\n");
        printf("Pedido encontrado:\n");
        printf("ID Pedido: %d\n", pedidosMesa[i].id);
        printf("ID Mesa: %d\n", pedidosMesa[i].mesa.id);
        printf("Capacidad: %d\n", pedidosMesa[i].mesa.capacidad);
        printf("Ubicacion: %s\n", pedidosMesa[i].mesa.ubicacion);
        printf("Disponibilidad: %s\n", pedidosMesa[i].estado);
        printf("Fecha y hora: %s\n", pedidosMesa[i].fechaHora);
        printf("-------------------------------------\n");
    }

    free(pedidosMesa);
}
// void calcularTotalPedidoMenu()
// {
//     int idPedido;
//     printf("Ingrese el ID del pedido a calcular: ");
//     scanf("%d", &idPedido);

//     float total = calcularTotalPedido("detalles_pedido.dat", idPedido);

//     if (total > 0.0f)
//     {
//         printf("El total del pedido %d es: %.2f\n", idPedido, total);
//     }
//     else
//     {
//         printf("No se pudo calcular el total del pedido.\n");
//     }
// }

void calcularTotalPedidoMenu()
{
    int idPedido;
    float totalPedido;
    printf("Ingrese el ID del pedido a calcular: ");
    scanf("%d", &idPedido);
    totalPedido = calcularTotalPedido(FILE_DETALLES, idPedido);
    if (totalPedido > 0.0f)
    {
        printf("El total del pedido %d es: %.2f\n", idPedido, totalPedido);
    }
    else
    {
        printf("No se pudo calcular el total del pedido.\n");
    }
}
void mostrarPedidosConTotales()
{
    int numPedidos;
    Pedido *pedidos = cargarPedidos(FILE_PEDIDO, &numPedidos);

    if (!pedidos || numPedidos == 0)
    {
        printf("No hay pedidos registrados.\n");
        return;
    }

    printf("\nListado de Pedidos con Totales:\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < numPedidos; i++)
    {
        float total = calcularTotalPedido(FILE_DETALLES, pedidos[i].id);
        printf("Pedido ID: %d\n", pedidos[i].id);
        printf("Mesa ID: %d\n", pedidos[i].mesa.id);
        printf("Fecha: %s\n", pedidos[i].fechaHora);
        printf("Estado: %s\n", pedidos[i].estado);
        printf("Total a pagar: %.2f\n", total);
        printf("-------------------------------------------------\n");
    }

    free(pedidos);
}