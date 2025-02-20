#include <stdio.h>
#include <conio.h>
#include "repositories.h"
#include "mesa-servicio.h"
#include "producto-servicio.h"
#include "login-servicio.h"
#include "pedido-servicio.h"
#include "detalle-servicio.h"
#include <locale.h>
#include <windows.h>
#include <stdbool.h>

void mainMenu();
void inputMenuProductos();
void inputMenuMesas();
void inputMenuPedido();
void inputMenuDetalle();

int main()
{
    //inputMenuLogin();
    mainMenu();
    return 0;
}

void mainMenu()
{
    int opcion;

    do
    {
        system("cls");
        printf("\nMenu Principal\n");
        printf("1. Ventas \n");
        printf("2. Pedidos\n");
        printf("3. Gestion Productos\n");
        printf("4. Gestion Mesas\n");
        printf("5. Salir\n");
        int opcion = validarNum("Ingrese una opcion: ");
        switch (opcion)
        {
        case 1:
            inputMenuDetalle();
            break;
        case 2:
            inputMenuPedido();
            break;
        case 3:
            inputMenuProductos();
            break;
        case 4:
            inputMenuMesas();
            break;
        case 5:
            system("cls");
            printf("Saliendo del programa...\n");
            Sleep(2000);
            exit(0);
            break;
        default:
            printf("Opcion invalida. Por favor, ingrese un numero entre 1 y 5.\n");
        }
    } while (opcion != 5);
}

// Funcion selector del menu Mesas
void inputMenuMesas()
{
    int opcion;

    do
    {
        
        system("cls");
        mostrarMenuMesas();
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            accionMostrarDisponibilidad();
            break;
        case 2:
            accionReservarMesa();
            break;
        case 3:
            accionLiberarMesa();
            break;
        case 4:
            accionAgregarMesa();
            break;
        case 5:
            accionBuscarMesaPorId();
            break;
        case 6:
            printf("\nVolviendo...\n");
            break;
        default:
            printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 6);
}

// Funcion selector del menu Productos
void inputMenuProductos()
{
    int opcion;

    do
    {
        
        system("cls");
        mostrarMenuProducto();
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            mostrarTodosLosProductos();
            break;
        case 2:
            modificarProductoMenu();
            break;
        case 3:
            modificarStockProductoMenu();
            break;
        case 4:
            agregarProductoMenu();
            break;
        case 5:
            buscarProductoMenu();
            break;
        case 6:
            printf("\nVolviendo...\n");
            break;
        default:
            printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 6);
}

// Funcion selector del menu Pedidos
void inputMenuPedido()
{
    int opcion;
    do
    {
        
        system("cls");
        mostrarMenuPedido();
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            agregarPedidoMenu();
            break;

        case 2:
            buscarPedidoMenu();
            break;

        case 3:
            modificarPedidoMenu();
            break;

        case 4:
            eliminarPedidoMenu();
            break;

        case 5:
            buscarPedidosPorMesaMenu();
            break;

        case 6:
            mostrarPedidosConTotales();
            break;

        case 7:
            printf("\nVolviendo...\n");
            break;

        default:
            printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 7);
}
// Funcion selector del menu Pedidos
void inputMenuDetalle()
{
    int opcion;
    do
    {
        
        system("cls");
        mostrarMenuDetalle();
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            agregarDetallePedidoService();
            break;

        case 2:
            calcularTotalPedidoMenu();
            break;
        case 3:
            mostrarDetallesPedido();
            break;
        case 4:
            printf("\nVolviendo...\n");
            break;
        default:
            printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 4);
}