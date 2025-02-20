#include "utilities.h"

//Funcion Auxiliar
int validarNum(const char *mensaje) {
    int num;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%d", &num) != 1 || num < 0) {
            printf("Error: Ingrese un numero entero valido.\n");
            while (getchar() != '\n'); // Limpiar el buffer
        } else {
            return num;
        }
    }
}
void esperarTecla() {
    printf("\nPresione Enter para continuar...");
    while (getchar() != '\n'); // Limpiar buffer si hay caracteres pendientes
    getchar(); // Esperar que el usuario presione Enter
}