#include "pedido-servicio.h"
#include "mesa-servicio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

void mostrarMenuPedido()
{
    crearArchivoPedidos();
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
void agregarPedidoMenu()
{
    Pedido nuevoPedido;
    getchar();
    // Obtener el último ID registrado
    int ultimoId = obtenerUltimoIdPedido(FILE_PEDIDO);
    nuevoPedido.id = (ultimoId == -1) ? 0 : ultimoId + 1;
    printf("ID: %d\n", nuevoPedido.id);
    // Solicitar ID de la mesa asociada al pedido
    int idMesa;
    idMesa=validarNum("Ingrese el ID de la mesa: ");
    // Buscar la mesa por ID en el archivo binario
    Mesa mesaEncontrada = buscarMesaPorId(FILE_MESAS, idMesa);
    if (mesaEncontrada.id == 0) {
        printf("No se encontró ninguna mesa con el ID %d. Pedido no agregado.\n", idMesa);
        return;
    }

    nuevoPedido.mesa = mesaEncontrada;

    while (getchar() != '\n');

    // Obtener la fecha y hora actual
    time_t now = time(0);
    struct tm *tlocal = localtime(&now);
    char output[sizeof "YYYY-MM-DD HH:MM:SS"];
    strftime(output, sizeof(output), "%Y-%m-%d %H:%M:%S", tlocal);
    strcpy(nuevoPedido.fechaHora, output);

    // Obtener el estado del pedido desde el usuario
    int estado;
    do {
        printf("Ingrese el estado del pedido:\n");
        printf("1. Pendiente\n");
        printf("2. En proceso\n");
        printf("3. Completado\n");
        if (scanf("%d", &estado) != 1) {
            printf("Error: Ingrese un número válido.\n");
            while (getchar() != '\n'); // Limpiar el buffer de entrada
            estado = 0; // Asegurar que se mantenga en el bucle
        }
    } while (estado < 1 || estado > 3);

    switch (estado) {
    case 1:strcpy(nuevoPedido.estado, "Pendiente");break;
    case 2:strcpy(nuevoPedido.estado, "En proceso");break;
    case 3:strcpy(nuevoPedido.estado, "Completado");break;
    }

    // Guardar el pedido en el archivo binario
    if (agregarPedido(FILE_PEDIDO, nuevoPedido)) {
        printf("Pedido agregado correctamente con ID %d.\n", nuevoPedido.id);
        esperarTecla();
    } else {
        printf("Error al agregar el pedido.\n");
        esperarTecla();
    }
}
void buscarPedidoMenu()
{
    int idPedido;
    idPedido = validarNum("Ingrese el ID del pedido a buscar: ");
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
        esperarTecla();
    }
    else
    {
        printf("No se encontro ningun pedido con el ID %d.\n", idPedido);
        esperarTecla();
    }
}
void modificarPedidoMenu()
{
    int id, idMesaAux;
    Pedido pedidoActual, nuevoPedido;
    id=validarNum("Ingrese el ID del pedido a modificar: ");
    pedidoActual = buscarPedidoPorId(FILE_PEDIDO, id);
    if (pedidoActual.id == 0)
    {
        printf("Pedido no encontrado\n");
        return;
    }
    while (getchar() != '\n'); // Limpiar buffer después de scanf
    // Copiar los datos del pedido actual al nuevo producto
    idMesaAux = pedidoActual.mesa.id;
    nuevoPedido = pedidoActual;
    printf("Ingrese los nuevos datos del pedido:\n");
    int estado;
    
    do {
        printf("Estado: \n");
        printf("1. Pendiente\n");
        printf("2. En proceso\n");
        printf("3. Completado\n");
        if (scanf("%d", &estado) != 1) {
            printf("Error: Ingrese un número válido.\n");
            while (getchar() != '\n'); // Limpiar el buffer de entrada
            estado = 0; // Asegurar que se mantenga en el bucle
        }
    } while (estado < 1 || estado > 3);
    switch (estado) {
    case 1:
        strcpy(nuevoPedido.estado, "Pendiente");
        break;
    case 2:
        strcpy(nuevoPedido.estado, "En proceso");
        break;
    case 3:
        strcpy(nuevoPedido.estado, "Completado");
        break;
    }
    // fgets(nuevoPedido.estado, sizeof(nuevoPedido.estado), stdin);
    // // Eliminar el salto de línea al final del Estado, si existe
    // nuevoPedido.estado[strcspn(nuevoPedido.estado, "\n")] = '\0';

    while (1) {
        printf("ID de la mesa: ");
        if (scanf("%d", &nuevoPedido.mesa.id) != 1) {
            printf("Error: Ingrese un número válido.\n");
            while (getchar() != '\n'); // Limpiar el buffer de entrada
        } else {
            break;
        }
    }
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
            esperarTecla();
        }
        else
        {
            printf("Pedido modificado correctamente\n");
            esperarTecla();
        }
    }
    else
    {
        printf("No se realizaron cambios en el pedido\n");
        esperarTecla();
    }
}
void eliminarPedidoMenu()
{
    int id;
    id=validarNum("Ingrese el ID del pedido a eliminar: ");
    if (eliminarPedido(FILE_PEDIDO, id))
    {
        printf("Pedido eliminado correctamente.\n");
        esperarTecla();
    }
    else
    {
        printf("Error: No se pudo eliminar el pedido.\n");
        esperarTecla();
    }
}
void buscarPedidosPorMesaMenu()
{
    int idMesa;
    idMesa=validarNum("Ingrese el ID de la mesa a consultar: ");
    int numPedidos;
    Pedido *pedidosMesa = buscarPedidosPorMesa(FILE_PEDIDO, idMesa, &numPedidos);

    if (pedidosMesa == NULL)
    {
        printf("No se encontraron pedidos para la mesa %d.\n", idMesa);
        esperarTecla();
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
    esperarTecla();
}
void calcularTotalPedidoMenu()
{
    int idPedido;
    float totalPedido;
    idPedido=validarNum("Ingrese el ID del pedido a calcular: ");
    totalPedido = calcularTotalPedido(FILE_DETALLES, idPedido);
    if (totalPedido > 0.0f)
    {
        printf("El total del pedido %d es: %.2f\n", idPedido, totalPedido);
        esperarTecla();
    }
    else
    {
        printf("No se pudo calcular el total del pedido.\n");
        esperarTecla();
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
    esperarTecla();
}