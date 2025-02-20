#include "producto-servicio.h"

#define FILE_PRODUCTOS "productos.bin"

// Funcion para mostrar el menu
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
            return 0; // Contiene al menos un caracter no vacio
        }
        cadena++;
    }
    return 1; // Solo tiene espacios o esta vacia
}
// Funcion para obtener entrada de texto valida
void obtenerEntradaValida(char *buffer, size_t size, const char *mensaje) {
    do {
        printf("%s", mensaje);
        fgets(buffer, size, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Eliminar el salto de linea
    } while (esCadenaVacia(buffer)); // Repetir si la cadena esta vacia
}
// Funcion para validar numeros flotantes (precio)
float validarFloat(const char *mensaje) {
    float num;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%f", &num) != 1 || num < 0) {
            printf("Error: Ingrese un numero valido.\n");
            while (getchar() != '\n'); // Limpiar el buffer
        } else {
            while (getchar() != '\n'); // Limpiar buffer despues de un ingreso correcto
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
            printf("Error: Ingrese un numero valido.\n");
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
        esperarTecla();
        return;
    }
    while (getchar() != '\n'); // Limpiar buffer despues de scanf
    // Copiar los datos del producto actual al nuevo producto
    nuevoProducto = productoActual;

    obtenerEntradaValida(nuevoProducto.nombre, sizeof(nuevoProducto.nombre), "Ingrese el nuevo nombre del producto: ");
    obtenerEntradaValida(nuevoProducto.descripcion, sizeof(nuevoProducto.descripcion), "Ingrese la nueva descripcion: ");
    nuevoProducto.precio = validarFloat("Ingrese el nuevo precio del producto: ");
    nuevoProducto.stock = validarNum("Ingrese el nuevo stock del producto: ");
    obtenerEntradaValida(nuevoProducto.categoria, sizeof(nuevoProducto.categoria), "Ingrese la nueva categoria: ");

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
    Producto nuevoProducto, productoAuxiliar;
    printf("\nIngrese los datos del nuevo producto:\n");
    nuevoProducto.id=validarNum("ID: ");
    productoAuxiliar = buscarProductoPorId(FILE_PRODUCTOS, nuevoProducto.id);

    if (productoAuxiliar.id != nuevoProducto.id)
    {
        obtenerEntradaValida(nuevoProducto.nombre, sizeof(nuevoProducto.nombre), "Nombre: ");
        obtenerEntradaValida(nuevoProducto.descripcion, sizeof(nuevoProducto.descripcion), "Descripcion: ");
        nuevoProducto.precio = validarFloat("Ingrese el nuevo precio del producto: ");
        nuevoProducto.stock = validarNum("Ingrese el nuevo stock del producto: ");
        obtenerEntradaValida(nuevoProducto.categoria, sizeof(nuevoProducto.categoria), "Ingrese la nueva categoria: ");
        if (!agregarProducto(FILE_PRODUCTOS, nuevoProducto))
        {
            printf("Error al agregar el producto\n");
            esperarTecla();
        }
        else
        {
            printf("Producto agregado correctamente\n");
            esperarTecla();
        }
    }else{
        printf("Id existente.");
        esperarTecla();
    }

}
void buscarProductoMenu()
{
    Producto prod;
    int id;
    id=validarNum("\nIngrese el ID del producto:");
    prod = buscarProductoPorId(FILE_PRODUCTOS, id);
    printf("%-5s %-20s %-20s %8s %14s %12s\n", "ID", "Nombre", "Descripcion", "Precio", "Categoria", "Stock");
    printf("%-5d %-20s %-20s %10.2f %10s %10d\n", prod.id, prod.nombre, prod.descripcion, prod.precio, prod.categoria, prod.stock);
    esperarTecla();
}