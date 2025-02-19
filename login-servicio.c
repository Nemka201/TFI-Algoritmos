#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "login-servicio.h"
#include "repositories.h"
#include <conio.h>

#define MAX_USUARIOS 100

// Funcion para mostrar el menu de login
void mostrarMenuLogin() {
    printf("\n--- Menu de Login ---\n");
    printf("1. Iniciar sesion\n");
    printf("2. Registrar un nuevo usuario\n");
    printf("3. Salir\n");
    printf("Ingrese una opcion: ");
}
// Funcion para manejar menu de Login
void inputMenuLogin(){
    Usuario usuarios[MAX_USUARIOS];
    int cantidadUsuarios = cargarUsuarios(usuarios, MAX_USUARIOS);
    
    int loginExitoso = 0;
    while (!loginExitoso) {
        mostrarMenuLogin();
        int opcionLogin;
    
        opcionLogin = validarNum("");
    
        if (opcionLogin == 1) {
            loginExitoso = iniciarSesion(usuarios, cantidadUsuarios);
        }
        else if (opcionLogin == 2) {
            registrarUsuario(usuarios, &cantidadUsuarios, MAX_USUARIOS);
            guardarUsuarios(usuarios, cantidadUsuarios);
            printf("\n");
        } else {
            exit(0);
        }
    }
}

// Funcion para iniciar sesion
int iniciarSesion(Usuario usuarios[], int cantidadUsuarios) {
    char nombreUsuario[11];
    char contrasena[33];
    
    printf("Ingrese el nombre de usuario: ");
    scanf("%s", nombreUsuario);
    printf("Ingrese la contrasena: ");
    scanf("%s", contrasena);
    
    // Buscar al usuario
    for (int i = 0; i < cantidadUsuarios; i++) {
        if (strcmp(usuarios[i].username, nombreUsuario) == 0 &&
            strcmp(usuarios[i].password, contrasena) == 0) {
            printf("Inicio de sesion exitoso.\n");
            return 1;
        }
    }
    printf("Credenciales incorrectas.\n");
    return 0;
}

// Funcion para validar el nombre de usuario
int validarNombreUsuario(const char* nombreUsuario, Usuario usuarios[], int cantidadUsuarios) {
    // Validacion de longitud
    int len = strlen(nombreUsuario);
    if (len < 6 || len > 10) {
        printf("El nombre de usuario debe tener entre 6 y 10 caracteres.\n");
        return 0;
    }
    
    // Validacion de inicio con minuscula
    if (!islower(nombreUsuario[0])) {
        printf("El nombre de usuario debe comenzar con una letra minuscula.\n");
        return 0;
    }
    
    // Validacion de letras mayusculas
    int mayusculas = 0;
    for (int i = 0; i < len; i++) {
        if (isupper(nombreUsuario[i])) {
            mayusculas++;
        }
    }
    if (mayusculas < 2) {
        printf("El nombre de usuario debe tener al menos 2 letras mayusculas.\n");
        return 0;
    }
    
    // Validacion de cantidad de digitos
    int digitos = 0;
    for (int i = 0; i < len; i++) {
        if (isdigit(nombreUsuario[i])) {
            digitos++;
        }
    }
    if (digitos > 3) {
        printf("El nombre de usuario no puede tener mas de 3 digitos.\n");
        return 0;
    }
    
    // Verificar si es unico
    for (int i = 0; i < cantidadUsuarios; i++) {
        if (strcmp(usuarios[i].username, nombreUsuario) == 0) {
            printf("El nombre de usuario ya esta registrado.\n");
            return 0;
        }
    }
    
    return 1;
}

// Funcion para validar la contrasena
int validarContrasena(const char* contrasena) {
    int len = strlen(contrasena);

    if (len < 6 || len > 32) {
        printf("La contrasena debe tener entre 6 y 32 caracteres.\n");
        return 0;
    }

    int mayuscula = 0, minuscula = 0, numero = 0;
    for (int i = 0; i < len; i++) {
        if (isupper(contrasena[i])) mayuscula = 1;
        if (islower(contrasena[i])) minuscula = 1;
        if (isdigit(contrasena[i])) numero = 1;
    }

    if (!mayuscula || !minuscula || !numero) {
        printf("La contrasena debe contener al menos una letra mayuscula, una minuscula y un numero.\n");
        return 0;
    }

    for (int i = 0; i < len; i++) {
        if (!isalnum(contrasena[i])) {
            printf("La contrasena solo puede contener caracteres alfanumericos.\n");
            return 0;
        }
    }

    for (int i = 0; i < len - 2; i++) {
        if (isdigit(contrasena[i]) && isdigit(contrasena[i+1]) && isdigit(contrasena[i+2])) {
            printf("La contrasena no puede tener mas de 3 digitos consecutivos.\n");
            return 0;
        }
    }

    for (int i = 0; i < len - 1; i++) {
        if (tolower(contrasena[i]) == tolower(contrasena[i+1]) - 1) {
            printf("La contrasena no debe contener letras consecutivas.\n");
            return 0;
        }
    }

    return 1;
}

// Funcion para registrar un nuevo usuario
void registrarUsuario(Usuario usuarios[], int* cantidadUsuarios, int maxUsuarios) {
    if (*cantidadUsuarios >= maxUsuarios) {
        printf("No se pueden registrar mas usuarios.\n");
        return;
    }

    char nombreUsuario[10];
    char contrasena[10];
    char nombreCompleto[60];

    printf("Ingrese un nombre de usuario: ");
    while (getchar() != '\n');
    scanf("%s", nombreUsuario);

    if (!validarNombreUsuario(nombreUsuario, usuarios, *cantidadUsuarios)) {
        return;
    }

    printf("Ingrese una contrasena: ");
    while (getchar() != '\n');
    scanf("%s", contrasena);

    if (!validarContrasena(contrasena)) {
        return;
    }

    printf("Ingrese su nombre completo: ");
    while (getchar() != '\n');
    scanf("%s", nombreCompleto);
    
    strcpy(usuarios[*cantidadUsuarios].username, nombreUsuario);
    strcpy(usuarios[*cantidadUsuarios].password, contrasena);
    strcpy(usuarios[*cantidadUsuarios].nombreCompleto, nombreCompleto);
    (*cantidadUsuarios)++;

    printf("Usuario registrado exitosamente.\n");
}
