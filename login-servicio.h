// login-servicio.h

#ifndef LOGIN_SERVICIO_H
#define LOGIN_SERVICIO_H

#include "repositories.h"

// Declaración de las funciones
int iniciarSesion(Usuario usuarios[], int cantidadUsuarios);
int validarNombreUsuario(const char* nombreUsuario, Usuario usuarios[], int cantidadUsuarios);
int validarContraseña(const char* contrasena);
void registrarUsuario(Usuario usuarios[], int* cantidadUsuarios, int maxUsuarios);
void mostrarMenuLogin();

#endif
