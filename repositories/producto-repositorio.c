#include "producto-repositorio.h"

// Funcion para guardar productos en un archivo binario
int guardarProductos(const char *nombreArchivo, Producto *productos, int numProductos)
{
    FILE *fp = fopen(nombreArchivo, "wb");
    if (fp == NULL)
    {
        perror("Error al abrir el archivo");
        return 0; // Indicar error
    }

    // Escribir un encabezado con el numero de productos para facilitar la carga posterior
    if (fwrite(&numProductos, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return 0; // Indicar error
    }

    if (fwrite(productos, sizeof(Producto), numProductos, fp) != numProductos)
    {
        fclose(fp);
        return 0; // Indicar error
    }

    fclose(fp);
    return 1; // Indicar exito
}

// Funcion para cargar productos desde un archivo binario
Producto *cargarProductos(const char *nombreArchivo, int *numProductos)
{
    FILE *fp = fopen(nombreArchivo, "rb");
    if (fp == NULL)
    {
        perror("Error al abrir el archivo");
        return NULL;
    }

    // Leer el numero de productos del encabezado
    fread(numProductos, sizeof(int), 1, fp);

    Producto *productos = (Producto *)malloc(*numProductos * sizeof(Producto));
    fread(productos, sizeof(Producto), *numProductos, fp);
    fclose(fp);
    return productos;
}

// Funcion para buscar un producto por su ID
Producto buscarProductoPorId(const char *nombreArchivo, int id)
{
    Producto productoEncontrado = {0};
    int numProductos;
    Producto *productos = cargarProductos(nombreArchivo, &numProductos);

    // Busqueda secuencial
    if (productos)
    {
        for (int i = 0; i < numProductos; i++)
        {
            if (productos[i].id == id)
            {
                productoEncontrado = productos[i];
                break;
            }
        }
        free(productos);
    }
    return productoEncontrado;
}

// Funcion para agregar un nuevo producto
int agregarProducto(const char *nombreArchivo, Producto nuevoProducto)
{
    int numProductos = 0;
    Producto *productos = cargarProductos(nombreArchivo, &numProductos);

    // Crear un nuevo bloque de memoria para los productos
    Producto *tempProductos = (Producto *)realloc(productos, (numProductos + 1) * sizeof(Producto));
    if (tempProductos == NULL)
    {
        perror("Error al realocar memoria");
        free(productos); // Liberar memoria original antes de salir
        return 0;        // Indicar error
    }

    productos = tempProductos; // Asignar el nuevo puntero si realloc fue exitoso

    // Agregar el nuevo producto
    productos[numProductos] = nuevoProducto;
    numProductos++;

    // Guardar la lista actualizada en el archivo
    if (!guardarProductos(nombreArchivo, productos, numProductos))
    {
        free(productos);
        return 0; // Indicar error si no se pudo guardar
    }

    free(productos);
    return 1; // Indicar exito
}

// Funcion para modificar un producto existente
int modificarProducto(const char *nombreArchivo, int id, Producto nuevoProducto)
{
    int numProductos;
    Producto *productos = cargarProductos(nombreArchivo, &numProductos);

    for (int i = 0; i < numProductos; i++)
    {
        if (productos[i].id == id)
        {
            productos[i] = nuevoProducto;
            guardarProductos(nombreArchivo, productos, numProductos);
            free(productos);
            return 1; // Modificacion exitosa
        }
    }

    free(productos);
    return 0; // Producto no encontrado
}

// Funcion para eliminar un producto
int eliminarProducto(const char *nombreArchivo, int id)
{
    int numProductos;
    Producto *productos = cargarProductos(nombreArchivo, &numProductos);

    int indiceEliminar = -1;
    for (int i = 0; i < numProductos; i++)
    {
        if (productos[i].id == id)
        {
            indiceEliminar = i;
            break;
        }
    }

    if (indiceEliminar != -1)
    {
        // Mover los elementos a la izquierda del elemento eliminado
        for (int i = indiceEliminar; i < numProductos - 1; i++)
        {
            productos[i] = productos[i + 1];
        }

        // Reducir el tamaño del arreglo
        numProductos--;

        // Guardar los productos actualizados
        guardarProductos(nombreArchivo, productos, numProductos);

        free(productos);
        return 1; // Eliminacion exitosa
    }

    free(productos);
    return 0; // Producto no encontrado
}

// Funcion para buscar productos por categoria
Producto *buscarProductosPorCategoria(const char *nombreArchivo, const char *categoria)
{
    int numProductos;
    Producto *productos = cargarProductos(nombreArchivo, &numProductos);

    // Crear un nuevo arreglo para almacenar los productos encontrados
    int numProductosEncontrados = 0;
    for (int i = 0; i < numProductos; i++)
    {
        if (strcmp(productos[i].categoria, categoria) == 0)
        {
            numProductosEncontrados++;
        }
    }

    Producto *productosEncontrados = (Producto *)malloc(numProductosEncontrados * sizeof(Producto));
    int indice = 0;
    for (int i = 0; i < numProductos; i++)
    {
        if (strcmp(productos[i].categoria, categoria) == 0)
        {
            productosEncontrados[indice++] = productos[i];
        }
    }

    free(productos);
    return productosEncontrados;
}