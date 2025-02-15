#include "producto-servicio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    printf("Ingrese el ID del producto a modificar: ");
    scanf("%d", &id);

    Producto productoActual = buscarProductoPorId(FILE_PRODUCTOS, id);
    if (productoActual.id == 0)
    {
        printf("Producto no encontrado\n");
        return;
    }

    printf("Nuevo stock para el producto %s: ", productoActual.nombre);
    scanf("%d", &nuevaCantidad);

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

    printf("Ingrese el ID del producto a modificar: ");
    scanf("%d", &id);

    productoActual = buscarProductoPorId(FILE_PRODUCTOS, id);
    if (productoActual.id == 0)
    {
        printf("Producto no encontrado\n");
        return;
    }

    // Copiar los datos del producto actual al nuevo producto
    nuevoProducto = productoActual;
    printf("Ingrese los nuevos datos del producto:\n");
    printf("Nombre: ");
    fgets(nuevoProducto.nombre, sizeof(nuevoProducto.nombre), stdin);
    // Eliminar el salto de línea al final del nombre, si existe
    nuevoProducto.nombre[strcspn(nuevoProducto.nombre, "\n")] = '\0';

    printf("Descripcion: ");
    // Descartar el resto de la línea, incluyendo el salto de línea pendiente
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    fgets(nuevoProducto.descripcion, sizeof(nuevoProducto.descripcion), stdin);
    nuevoProducto.descripcion[strcspn(nuevoProducto.descripcion, "\n")] = '\0';

    printf("Precio: ");
    scanf("%f", &nuevoProducto.precio);

    printf("Stock: ");
    scanf("%d", &nuevoProducto.stock);

    printf("Categoría: ");
    fgets(nuevoProducto.categoria, sizeof(nuevoProducto.categoria), stdin);
    nuevoProducto.categoria[strcspn(nuevoProducto.categoria, "\n")] = '\0';

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

    printf("ID: ");
    scanf("%d", &nuevoProducto.id);
    while (getchar() != '\n'); // Limpiar buffer después de scanf
    printf("Nombre: ");
    fgets(nuevoProducto.nombre, sizeof(nuevoProducto.nombre), stdin);
    // Eliminar el salto de línea al final del nombre, si existe
    nuevoProducto.nombre[strcspn(nuevoProducto.nombre, "\n")] = '\0';

    printf("Descripcion: ");
    nuevoProducto.descripcion[strcspn(nuevoProducto.descripcion, "\n")] = '\0';


    fgets(nuevoProducto.descripcion, sizeof(nuevoProducto.descripcion), stdin);
    nuevoProducto.descripcion[strcspn(nuevoProducto.descripcion, "\n")] = '\0';

    printf("Precio: ");
    scanf("%f", &nuevoProducto.precio);

    printf("Stock: ");
    scanf("%d", &nuevoProducto.stock);

    printf("Categoria (1: Bebidas, 2: Alimentos): ");
    int categoria;
    scanf("%d", &categoria);
    if (categoria == 1)
    {
        strcpy(nuevoProducto.categoria, "Bebidas");
    }
    else if (categoria == 2)
    {
        strcpy(nuevoProducto.categoria, "Alimentos");
    }
    else
    {
        printf("Categoría inválida\n");
        return;
    }

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
    printf("\nIngrese el ID del producto:");
    scanf("%d", &id);
    prod = buscarProductoPorId(FILE_PRODUCTOS, id);
}