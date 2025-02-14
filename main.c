#include <stdio.h>
#include "repositories.h"
#include "mesa-servicio.h"

void mostrarMenu();
void mostrarMenuMesas();
void accionMostrarDisponibilidad();
void accionReservarMesa();
void accionLiberarMesa();
void accionAgregarMesa();
void accionBuscarMesaPorId();
void inputMenuMesas();
void MainMenu();

int main() {
    int i;
	MainMenu();
    
    return 0;
}

void MainMenu() {
    int opcion;

    do {
        printf("\nMenu Principal\n");
        printf("1. Opcion 1\n");
        printf("2. Opcion 2\n");
        printf("3. Opcion 3\n");
        printf("4. Gestion de mesas\n");
        printf("5. Salir\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                // Código para la opción 1
                printf("Has seleccionado la opcion 1\n");
                break;
            case 2:
                // Código para la opción 2
                printf("Has seleccionado la opcion 2\n");
                break;
            case 3:
                // Código para la opción 3
                printf("Has seleccionado la opcion 3\n");
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
void inputMenuMesas() {
    int opcion;

    do {
        mostrarMenuMesas();
        scanf("%d", &opcion);

        switch (opcion) {
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
                printf("Saliendo...\n");
                break;

            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (opcion != 6);
}