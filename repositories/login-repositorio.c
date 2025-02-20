#include "../repositories/login-repositorio.h"

// Funcion para cargar los usuarios desde el archivo binario
int cargarUsuarios(Usuario usuarios[], int maxUsuarios) {
    FILE *file = fopen("Usuarios.dat", "rb");
    if (!file) {
        return 0; // No hay usuarios guardados, retorno 0
    }
    int cantidadUsuarios = fread(usuarios, sizeof(Usuario), maxUsuarios, file);
    fclose(file);
    return cantidadUsuarios;
}

// Funcion para guardar los usuarios en el archivo binario
void guardarUsuarios(Usuario usuarios[], int cantidadUsuarios) {
    FILE *file = fopen("Usuarios.dat", "wb");
    if (!file) {
        printf("Error al guardar los usuarios.\n");
        return;
    }
    fwrite(usuarios, sizeof(Usuario), cantidadUsuarios, file);
    fclose(file);
}