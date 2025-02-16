#include "pedido-servicio.h"
#include "mesa-servicio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define FILE_PEDIDO "pedido.bin"
#define FILE_MESAS "mesas.bin"

void mostrarMenuPedido() {
    printf("\n--- Menu de Gestion de Pedidos ---\n");
    printf("1. Guardar pedido\n");
    printf("2. Buscar pedido\n");
    printf("3. Editar pedido\n");
    //printf("4. Agregar una nueva mesa\n");
    //printf("5. Buscar una mesa por ID\n");
    printf("6. Volver\n");
    printf("Seleccione una opcion: ");
}

void agregarPedidoMenu() {
    Pedido nuevoPedido;
    getchar();

    // Solicitar ID del pedido
    printf("Ingrese el ID del pedido: ");
    scanf("%d", &nuevoPedido.id);

    // Solicitar ID de la mesa asociada al pedido
    printf("Ingrese el ID de la mesa: ");
    int idMesa;
    scanf("%d", &idMesa);

    // Buscar la mesa por ID en el archivo binario
    Mesa mesaEncontrada = buscarMesaPorId(FILE_MESAS, idMesa);
    
    // Verificar si la mesa existe
    if (mesaEncontrada.id == 0) {
        printf("No se encontró ninguna mesa con el ID %d. Pedido no agregado.\n", idMesa);
        return;
    }
    //Se ocupa la mesa
    reservarMesa(mesaEncontrada.id);
    // Asignar la mesa encontrada al pedido
    nuevoPedido.mesa = mesaEncontrada;
    getchar();
    printf("Ingrese la fecha y hora del pedido (YYYY-MM-DD HH:MM:SS): ");
    fgets(nuevoPedido.fechaHora, sizeof(nuevoPedido.fechaHora), stdin);
    nuevoPedido.fechaHora[strcspn(nuevoPedido.fechaHora, "\n")] = 0; // Eliminar el salto de línea

    printf("Ingrese el estado del pedido (Ejemplo: 'Pendiente', 'En proceso', 'Completado'): ");
    fgets(nuevoPedido.estado, sizeof(nuevoPedido.estado), stdin);
    nuevoPedido.estado[strcspn(nuevoPedido.estado, "\n")] = 0; // Eliminar el salto de línea


    // Llamar a la función que guarda el pedido en el archivo binario
    if (agregarPedido(FILE_PEDIDO, nuevoPedido)) {
        printf("Pedido agregado correctamente.\n");
    } else {
        printf("Error al agregar el pedido.\n");
    }
}

void buscarPedidoMenu(){
    int idPedido;
    printf("Ingrese el ID del pedido a buscar: ");
    scanf("%d", &idPedido);
    Pedido pedidoEncontrado = buscarPedidoPorId(FILE_PEDIDO, idPedido);

    if (pedidoEncontrado.id != 0) {
        printf("\n-------------------------------------\n");
        printf("Pedido encontrado:\n");
        printf("ID Pedido: %d\n", pedidoEncontrado.id);
        printf("ID Mesa: %d\n", pedidoEncontrado.mesa.id);
        printf("Capacidad: %d\n", pedidoEncontrado.mesa.capacidad);
        printf("Ubicacion: %s\n", pedidoEncontrado.mesa.ubicacion);
        printf("Disponibilidad: %s\n", pedidoEncontrado.estado);
        printf("Fecha y hora: %s\n", pedidoEncontrado.fechaHora);
        printf("-------------------------------------\n");
    } else {
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
    while (getchar() != '\n'); // Limpiar buffer después de scanf
    // Copiar los datos del pedido actual al nuevo producto
    idMesaAux= pedidoActual.mesa.id;
    nuevoPedido = pedidoActual;
    printf("Ingrese los nuevos datos del pedido:\n");
    printf("Estado: ");
    fgets(nuevoPedido.estado, sizeof(nuevoPedido.estado), stdin);
    // Eliminar el salto de línea al final del Estado, si existe
    nuevoPedido.estado[strcspn(nuevoPedido.estado, "\n")] = '\0';

    printf("ID de la mesa: ");
    scanf("%d", &nuevoPedido.mesa.id);
    if (nuevoPedido.mesa.id != idMesaAux){
        liberarMesa(idMesaAux);
    }
    
    // Solo si alguno de los datos fue modificado, actualizar el pedido
    if (strcmp(nuevoPedido.estado, pedidoActual.estado) != 0 ||
        nuevoPedido.mesa.id != pedidoActual.mesa.id )
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
