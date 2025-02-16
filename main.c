#include <stdio.h>
#include <conio.h>
#include "repositories.h"
#include "mesa-servicio.h"
#include "producto-servicio.h"
#include "login-servicio.h"
#include "pedido-servicio.h"

void MainMenu();
void inputMenuProductos();
void inputMenuMesas();
void inputMenuPedido();

#define MAX_USUARIOS 100

int main() {
    /*Usuario usuarios[MAX_USUARIOS];
    int cantidadUsuarios = cargarUsuarios(usuarios, MAX_USUARIOS);

    int loginExitoso = 0;
    while (!loginExitoso) {
        mostrarMenuLogin();
        int opcionLogin;
        
        scanf("%d", &opcionLogin);

        if (opcionLogin == 1) {
            loginExitoso = iniciarSesion(usuarios, cantidadUsuarios);
        }
        else if (opcionLogin == 2) {
            registrarUsuario(usuarios, &cantidadUsuarios, MAX_USUARIOS);
            guardarUsuarios(usuarios, cantidadUsuarios);
            printf("\n");
        }
    }*/

    MainMenu();
    return 0;
}


void MainMenu()
{
    int opcion;

    do
    {
        system("pause");
        system("cls");
        printf("\nMenu Principal\n");
        printf("1. Opcion 1\n");
        printf("2. Pedidos\n");
        printf("3. Gestion Productos\n");
        printf("4. Gestion de mesas\n");
        printf("5. Salir\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            printf("Has seleccionado la opcion 1\n");
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
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opcion invalida. Por favor, ingrese un numero entre 1 y 5.\n");
        }
    } while (opcion != 5);
}

// Función selector del menu Mesas
void inputMenuMesas()
{
    int opcion;

    do
    {
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
            printf("Opción no valida. Intente de nuevo.\n");
        }
    } while (opcion != 6);
}

// Función selector del menu Productos
void inputMenuProductos()
{
    int opcion;

    do
    {
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
            printf("Opción no valida. Intente de nuevo.\n");
        }
    } while (opcion != 6);
}

// Función selector del menu Pedidos
void inputMenuPedido()
{
    int opcion;
    do
    {
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
            break;

        case 6:
            printf("\nVolviendo...\n");
            break;

        default:
            printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (opcion != 6);
}
