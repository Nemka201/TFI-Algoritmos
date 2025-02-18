#include "mesa-servicio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define FILE_MESAS "mesas.bin"

// Función para reservar una mesa
int reservarMesa(int idMesa) {
    int numMesas;
    Mesa* mesas = cargarMesas(FILE_MESAS, &numMesas);

    for (int i = 0; i < numMesas; i++) {
        if (mesas[i].id == idMesa) {
            if (mesas[i].disponible) {
                mesas[i].disponible = false; // Marcar como reservada
                guardarMesas(FILE_MESAS, mesas, numMesas); // Guardar cambios
                free(mesas);
                printf("Mesa %d reservada con exito.\n", idMesa);
                return 1; // Reserva exitosa
            } else {
                free(mesas);
                printf("La mesa %d no esta disponible.\n", idMesa);
                return 0; // Mesa no disponible
            }
        }
    }

    free(mesas);
    printf("Error: No se encontró la mesa %d.\n", idMesa);
    return 0; // Mesa no encontrada
}
// Función para liberar una mesa
int liberarMesa(int idMesa) {
    int numMesas;
    Mesa* mesas = cargarMesas(FILE_MESAS, &numMesas);

    for (int i = 0; i < numMesas; i++) {
        if (mesas[i].id == idMesa) {
            if (!mesas[i].disponible) {
                mesas[i].disponible = true; // Marcar como disponible
                guardarMesas(FILE_MESAS, mesas, numMesas); // Guardar cambios
                free(mesas);
                printf("Mesa %d liberada con exito.\n", idMesa);
                return 1; // Liberación exitosa
            } else {
                free(mesas);
                printf("La mesa %d no esta reservada.\n", idMesa);
                return 0; // Mesa no reservada
            }
        }
    }

    free(mesas);
    printf("Error: No se encontró la mesa %d.\n", idMesa);
    return 0; // Mesa no encontrada
}
// Función para mostrar la disponibilidad de las mesas
void mostrarDisponibilidadMesas() {
    int numMesas;
    Mesa* mesas = cargarMesas(FILE_MESAS, &numMesas);

    printf("Disponibilidad de mesas:\n");
    for (int i = 0; i < numMesas; i++) {
        printf("\n-------------------------------------\n");
        printf("Mesa %d: %s\n", mesas[i].id, mesas[i].disponible ? "Disponible" : "Reservada");
        printf("-------------------------------------\n");
    }

    free(mesas); // Liberar la memoria asignada
}
// Función para mostrar el menú
void mostrarMenuMesas() {
    printf("\n--- Menu de Gestion de Mesas ---\n");
    printf("1. Mostrar disponibilidad de mesas\n");
    printf("2. Reservar una mesa\n");
    printf("3. Liberar una mesa\n");
    printf("4. Agregar una nueva mesa\n");
    printf("5. Buscar una mesa por ID\n");
    printf("6. Volver\n");
    printf("Seleccione una opcion: ");
}
// Función para mostrar la disponibilidad de las mesas
void accionMostrarDisponibilidad() {
    mostrarDisponibilidadMesas();
}
// Función para reservar una mesa
void accionReservarMesa() {
    int idMesa;
    idMesa = validarNum("Ingrese el ID de la mesa a reservar: ");
    reservarMesa(idMesa);
}
// Función para liberar una mesa
void accionLiberarMesa() {
    int idMesa;
    idMesa = validarNum("Ingrese el ID de la mesa a liberar: ");
    liberarMesa(idMesa);
}
// Función para agregar una nueva mesa
void accionAgregarMesa() {
    Mesa nuevaMesa;
    printf("Ingrese los datos de la nueva mesa:\n");
    nuevaMesa.id=validarNum("ID: ");
    nuevaMesa.capacidad=validarNum("Capacidad: ");
    printf("Seleccione la ubicacion:\n");
    printf("1. Terraza\n");
    printf("2. Planta-Baja\n");
    printf("3. Planta-Alta\n");
    
    int opcion;
    while (1) {
        if (scanf("%d", &opcion) != 1) {
            printf("Error: Ingrese un número válido.\n");
            printf("Seleccione una opcion: ");
            while (getchar() != '\n'); // Limpiar el buffer de entrada
        } else {
            break;
        }
        
    }
    
    switch(opcion) {
        case 1: strcpy(nuevaMesa.ubicacion, "Terraza"); break;
        case 2: strcpy(nuevaMesa.ubicacion, "Planta-Baja"); break;
        case 3: strcpy(nuevaMesa.ubicacion, "Planta-Alta"); break;
        default: 
            printf("Opcion inválida. Se asignara 'Planta-Baja' por defecto.\n");
            strcpy(nuevaMesa.ubicacion, "Planta-Baja");
    }
    nuevaMesa.disponible = true; // Por defecto, la mesa está disponible

    if (agregarMesa(FILE_MESAS, nuevaMesa)) {
        printf("Mesa agregada con exito.\n");
        esperarTecla();
    } else {
        printf("Error al agregar la mesa.\n");
        esperarTecla();
    }
}
// Función para buscar una mesa por ID
void accionBuscarMesaPorId() {
    int idMesa;
    idMesa=validarNum("Ingrese el ID de la mesa a buscar: ");
    Mesa mesaEncontrada = buscarMesaPorId(FILE_MESAS, idMesa);

    if (mesaEncontrada.id != 0) {
        printf("\n-------------------------------------\n");
        printf("Mesa encontrada:\n");
        printf("ID: %d\n", mesaEncontrada.id);
        printf("Capacidad: %d\n", mesaEncontrada.capacidad);
        printf("Ubicacion: %s\n", mesaEncontrada.ubicacion);
        printf("Disponibilidad: %s\n", mesaEncontrada.disponible ? "Disponible" : "Reservada");
        printf("-------------------------------------\n");
    } else {
        printf("No se encontro ninguna mesa con el ID %d.\n", idMesa);
    }
}

