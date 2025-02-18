#include "producto-servicio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define FILE_PRODUCTOS "productos.bin"

// Función para mostrar el menú
void mostrarMenuProducto()
{
    printf("\n-------------------------------------\n");
    printf("\n--- Menu de Gestion de productos ---\n");
    printf("1. Mostrar todos los productos\n");
    printf("2. Modificar un producto\n");
    printf("3. Agregar stock a un producto\n");
    printf("4. Agregar un nuevo producto\n");
    printf("5. Buscar un producto por ID\n");
    printf("6. Volver\n");
    printf("Seleccione una opcion: ");
}
int esCadenaVacia(const char *cadena) {
    while (*cadena) {
        if (!isspace((unsigned char)*cadena)) {
            return 0; // Contiene al menos un carácter no vacío
        }
        cadena++;
    }
    return 1; // Solo tiene espacios o está vacía
}
// Función para obtener entrada de texto válida
void obtenerEntradaValida(char *buffer, size_t size, const char *mensaje) {
    do {
        printf("%s", mensaje);
        fgets(buffer, size, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Eliminar el salto de línea
    } while (esCadenaVacia(buffer)); // Repetir si la cadena está vacía
}
// Función para validar números flotantes (precio)
float validarFloat(const char *mensaje) {
    float num;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%f", &num) != 1 || num < 0) {
            printf("Error: Ingrese un número válido.\n");
            while (getchar() != '\n'); // Limpiar el buffer
        } else {
            while (getchar() != '\n'); // Limpiar buffer después de un ingreso correcto
            return num;
        }
    }
}
void mostrarTodosLosProductos()
{
    int numProductos;
    Producto *productos = cargarProductos(FILE_PRODUCTOS, &numProductos);

    if (productos == NULL)
    {
        printf("Error al cargar los productos\n");
        return;
    }
    printf("\n-------------------------------------\n");
    printf("\nLista de productos:\n");
    printf("%-5s %-20s %-20s %10s %10s %10s\n", "ID", "Nombre", "Descripcion", "Precio", "Categoria", "Stock");
    for (int i = 0; i < numProductos; i++)
    {
        printf("%-5d %-20s %-20s %10.2f %10s %10d\n",
               productos[i].id,
               productos[i].nombre,
               productos[i].descripcion,
               productos[i].precio,
               productos[i].categoria,
               productos[i].stock);
    }

    free(productos);
}
void modificarStockProductoMenu()
{
    int id, nuevaCantidad;
    id = validarNum("Ingrese el ID del producto a modificar: ");

    Producto productoActual = buscarProductoPorId(FILE_PRODUCTOS, id);
    if (productoActual.id == 0)
    {
        printf("Producto no encontrado\n");
        return;
    }
    while (1)
    {
        printf("Nuevo stock para el producto %s: ", productoActual.nombre); 
        if (scanf("%d", &nuevaCantidad) != 1)
        {
            printf("Error: Ingrese un número válido.\n");
            while (getchar() != '\n')
                ; // Limpiar el buffer de entrada
        }
        else
        {
            break;
        }
    }

    productoActual.stock = nuevaCantidad;
    if (!modificarProducto(FILE_PRODUCTOS, id, productoActual))
    {
        printf("Error al modificar el producto\n");
    }
    else
    {
        printf("Producto modificado correctamente\n");
    }
}
void modificarProductoMenu()
{
    int id;
    Producto productoActual, nuevoProducto;
    id=validarNum("Ingrese el ID del producto a modificar: ");
    productoActual = buscarProductoPorId(FILE_PRODUCTOS, id);
    if (productoActual.id == 0)
    {
        printf("Producto no encontrado\n");
        return;
    }
    while (getchar() != '\n'); // Limpiar buffer después de scanf
    // Copiar los datos del producto actual al nuevo producto
    nuevoProducto = productoActual;

    obtenerEntradaValida(nuevoProducto.nombre, sizeof(nuevoProducto.nombre), "Ingrese el nuevo nombre del producto: ");
    obtenerEntradaValida(nuevoProducto.descripcion, sizeof(nuevoProducto.descripcion), "Ingrese la nueva descripción: ");
    nuevoProducto.precio = validarFloat("Ingrese el nuevo precio del producto: ");
    nuevoProducto.stock = validarNum("Ingrese el nuevo stock del producto: ");
    obtenerEntradaValida(nuevoProducto.categoria, sizeof(nuevoProducto.categoria), "Ingrese la nueva categoría: ");


    // printf("Ingrese los nuevos datos del producto:\n");
    // printf("Nombre: ");
    // fgets(nuevoProducto.nombre, sizeof(nuevoProducto.nombre), stdin);
    // // Eliminar el salto de línea al final del nombre, si existe
    // nuevoProducto.nombre[strcspn(nuevoProducto.nombre, "\n")] = '\0';

    // printf("Descripcion: ");
    // fgets(nuevoProducto.descripcion, sizeof(nuevoProducto.descripcion), stdin);
    // nuevoProducto.descripcion[strcspn(nuevoProducto.descripcion, "\n")] = '\0';

    // printf("Precio: ");
    // scanf("%f", &nuevoProducto.precio);

    // printf("Stock: ");
    // scanf("%d", &nuevoProducto.stock);

    // printf("Categoría: ");
    // fgets(nuevoProducto.categoria, sizeof(nuevoProducto.categoria), stdin);
    // nuevoProducto.categoria[strcspn(nuevoProducto.categoria, "\n")] = '\0';

    // Solo si alguno de los datos fue modificado, actualizar el producto
    if (strcmp(nuevoProducto.nombre, productoActual.nombre) != 0 ||
        strcmp(nuevoProducto.descripcion, productoActual.descripcion) != 0 ||
        nuevoProducto.precio != productoActual.precio ||
        nuevoProducto.stock != productoActual.stock ||
        strcmp(nuevoProducto.categoria, productoActual.categoria) != 0)
    {
        if (!modificarProducto(FILE_PRODUCTOS, id, nuevoProducto))
        {
            printf("Error al modificar el producto\n");
        }
        else
        {
            printf("Producto modificado correctamente\n");
        }
    }
    else
    {
        printf("No se realizaron cambios en el producto\n");
    }
}
void agregarProductoMenu()
{
    Producto nuevoProducto;
    printf("\nIngrese los datos del nuevo producto:\n");
    nuevoProducto.id=validarNum("ID: ");
    obtenerEntradaValida(nuevoProducto.nombre, sizeof(nuevoProducto.nombre), "Nombre: ");
    obtenerEntradaValida(nuevoProducto.descripcion, sizeof(nuevoProducto.descripcion), "Descripcion: ");
    nuevoProducto.precio = validarFloat("Ingrese el nuevo precio del producto: ");
    nuevoProducto.stock = validarNum("Ingrese el nuevo stock del producto: ");
    obtenerEntradaValida(nuevoProducto.categoria, sizeof(nuevoProducto.categoria), "Ingrese la nueva categoría: ");
     
    // printf("Nombre: ");
    // fgets(nuevoProducto.nombre, sizeof(nuevoProducto.nombre), stdin);
    // // Eliminar el salto de línea al final del nombre, si existe
    // nuevoProducto.nombre[strcspn(nuevoProducto.nombre, "\n")] = '\0';
    // printf("Descripcion: ");
    // nuevoProducto.descripcion[strcspn(nuevoProducto.descripcion, "\n")] = '\0';
    // fgets(nuevoProducto.descripcion, sizeof(nuevoProducto.descripcion), stdin);
    // nuevoProducto.descripcion[strcspn(nuevoProducto.descripcion, "\n")] = '\0';
    // printf("Precio: ");
    // scanf("%f", &nuevoProducto.precio);
    // printf("Stock: ");
    // scanf("%d", &nuevoProducto.stock);
    // printf("Categoria (1: Bebidas, 2: Alimentos): ");
    // int categoria;
    // scanf("%d", &categoria);
    // if (categoria == 1)
    // {
    //     strcpy(nuevoProducto.categoria, "Bebidas");
    // }
    // else if (categoria == 2)
    // {
    //     strcpy(nuevoProducto.categoria, "Alimentos");
    // }
    // else
    // {
    //     printf("Categoría inválida\n");
    //     return;
    // }

    if (!agregarProducto(FILE_PRODUCTOS, nuevoProducto))
    {
        printf("Error al agregar el producto\n");
    }
    else
    {
        printf("Producto agregado correctamente\n");
    }
}
void buscarProductoMenu()
{
    Producto prod;
    int id;
    id=validarNum("\nIngrese el ID del producto:");
    prod = buscarProductoPorId(FILE_PRODUCTOS, id);
}