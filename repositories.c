#include "repositories.h"

#define ARCHIVO_USUARIOS "Usuarios.dat"

//  MESA //

// Función para guardar un arreglo de mesas en un archivo binario
void guardarMesas(const char *nombreArchivo, Mesa *mesas, int numMesas)
{
    FILE *archivo = fopen(nombreArchivo, "wb");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        return;
    }

    fwrite(mesas, sizeof(Mesa), numMesas, archivo);

    fclose(archivo);
}

// Función para cargar un arreglo de mesas desde un archivo binario
Mesa *cargarMesas(const char *nombreArchivo, int *numMesas)
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        *numMesas = 0; // Inicializar numMesas a 0 en caso de error
        return NULL;
    }

    *numMesas = 0; // Inicializar a 0 antes de leer

    Mesa *mesas = NULL;
    Mesa mesa;
    while (fread(&mesa, sizeof(Mesa), 1, archivo) == 1)
    {
        (*numMesas)++;
        mesas = (Mesa *)realloc(mesas, (*numMesas) * sizeof(Mesa));
        if (mesas == NULL)
        {
            perror("Error al reasignar memoria: ");
            fclose(archivo);
            return NULL;
        }
        mesas[(*numMesas) - 1] = mesa;
    }

    fclose(archivo);
    return mesas;
}

// Función para buscar una mesa por su ID
Mesa buscarMesaPorId(const char *nombreArchivo, int id)
{
    Mesa mesaEncontrada;
    int numMesas;
    Mesa *mesas = cargarMesas(nombreArchivo, &numMesas);

    for (int i = 0; i < numMesas; i++)
    {
        if (mesas[i].id == id)
        {
            mesaEncontrada = mesas[i];
            break;
        }
    }

    free(mesas); // Liberar la memoria asignada
    return mesaEncontrada;
}

// Función para agregar una nueva mesa
int agregarMesa(const char *nombreArchivo, Mesa nuevaMesa)
{
    int numMesas;
    Mesa *mesas = cargarMesas(nombreArchivo, &numMesas);
    // Incrementar el tamaño del arreglo y agregar la nueva mesa
    numMesas++;
    mesas = (Mesa *)realloc(mesas, numMesas * sizeof(Mesa));
    mesas[numMesas - 1] = nuevaMesa;

    // Guardar el arreglo actualizado en el archivo
    guardarMesas(nombreArchivo, mesas, numMesas);

    free(mesas);
    return 1; // Indicar éxito
}


// Función auxiliar para encontrar el índice libre en un arreglo
int encontrarIndiceLibre(const char *nombreArchivo, int tamEstructura)
{
    // Para simplificar, asumimos que los registros se almacenan de forma consecutiva
    // y que no hay huecos en el archivo.
    // Si hay huecos, se necesitaría una lógica más compleja para encontrar el siguiente índice libre.

    FILE *archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        return -1;
    }

    fseek(archivo, 0, SEEK_END);
    long tamanoArchivo = ftell(archivo);
    fclose(archivo);

    return tamanoArchivo / tamEstructura;
}

// PEDIDOS //
#include <stdio.h>

void crearArchivoPedidos()
{
    FILE *fp = fopen("pedido.bin", "rb"); // Intentamos abrir el archivo en modo lectura

    if (!fp) // Si no existe, lo creamos
    {
        fp = fopen("pedido.bin", "wb"); // Crea el archivo en modo escritura binaria
        if (fp)
        {
            int numPedidos = 0;
            fwrite(&numPedidos, sizeof(int), 1, fp); // Escribe un 0 indicando que no hay pedidos
            fclose(fp);
            printf("Archivo 'pedido.bin' creado correctamente.\n");
        }
        else
        {
            printf("Error al crear el archivo 'pedido.bin'.\n");
        }
    }
    else
    {
        fclose(fp);
    }
}

int guardarPedidos(const char *nombreArchivo, Pedido *pedidos, int numPedidos)
{
    FILE *fp = fopen(nombreArchivo, "wb");
    if (!fp)
    {
        perror("Error al abrir el archivo");
        return 0;
    }
    fwrite(&numPedidos, sizeof(int), 1, fp);
    fwrite(pedidos, sizeof(Pedido), numPedidos, fp);
    fclose(fp);
    return 1;
}
Pedido *cargarPedidos(const char *nombreArchivo, int *numPedidos)
{
    FILE *fp = fopen(nombreArchivo, "rb");
    if (!fp)
    {
        perror("Error al abrir el archivo");
        *numPedidos = 0; // Importante: indicar que no hay pedidos
        return NULL;
    }

    // Verifica que se lea correctamente el número de pedidos
    if (fread(numPedidos, sizeof(int), 1, fp) != 1)  
    {
        perror("Error al leer la cantidad de pedidos o archivo vacío");
        fclose(fp);
        *numPedidos = 0;
        return NULL;
    }

    // Si el número de pedidos es inválido, evitar malloc con tamaño 0 o negativo
    if (*numPedidos <= 0)
    {
        printf("Advertencia: Archivo sin pedidos válidos.\n");
        fclose(fp);
        return NULL;
    }

    // Asignar memoria para los pedidos
    Pedido *pedidos = (Pedido *)malloc((*numPedidos) * sizeof(Pedido));
    if (!pedidos)
    {
        perror("Error al asignar memoria");
        fclose(fp);
        return NULL;
    }

    // Verifica que se lean correctamente los pedidos
    if (fread(pedidos, sizeof(Pedido), *numPedidos, fp) != (size_t)(*numPedidos))
    {
        perror("Error al leer los pedidos");
        free(pedidos);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return pedidos;
}

Pedido buscarPedidoPorId(const char *nombreArchivo, int id)
{
    int numPedidos;
    Pedido *pedidos = cargarPedidos(nombreArchivo, &numPedidos);
    Pedido pedidoEncontrado = {0};
    if (pedidos)
    {
        for (int i = 0; i < numPedidos; i++)
        {
            if (pedidos[i].id == id)
            {
                pedidoEncontrado = pedidos[i];
                break;
            }
        }
        free(pedidos);
    }
    return pedidoEncontrado;
}
int agregarPedido(const char *nombreArchivo, Pedido nuevoPedido)
{
    int numPedidos = 0;
    Pedido *pedidos = cargarPedidos(nombreArchivo, &numPedidos);

    // Si no hay pedidos, asignar memoria para el primero
    if (pedidos == NULL && numPedidos == 0) 
    {
        pedidos = (Pedido *)malloc(sizeof(Pedido));
        if (!pedidos)
        {
            perror("Error al asignar memoria para el primer pedido");
            return 0;
        }
        numPedidos = 0;
    }

    // Reservar más memoria para el nuevo pedido
    Pedido *nuevosPedidos = (Pedido *)realloc(pedidos, (numPedidos + 1) * sizeof(Pedido));
    if (!nuevosPedidos)
    {
        perror("Error al realocar memoria... ");
        free(pedidos);  // Solo liberar si realloc falló
        return 0;
    }

    // Agregar el nuevo pedido al array
    nuevosPedidos[numPedidos] = nuevoPedido;
    numPedidos++;

    // Guardar los pedidos en el archivo
    int resultado = guardarPedidos(nombreArchivo, nuevosPedidos, numPedidos);
    free(nuevosPedidos); // Liberar memoria después de guardar
    return resultado;
}


int modificarPedido(const char *nombreArchivo, int id, Pedido nuevoPedido)
{
    int numPedidos;
    Pedido *pedidos = cargarPedidos(nombreArchivo, &numPedidos);
    if (!pedidos)
        return 0;
    for (int i = 0; i < numPedidos; i++)
    {
        if (pedidos[i].id == id)
        {
            pedidos[i] = nuevoPedido;
            int resultado = guardarPedidos(nombreArchivo, pedidos, numPedidos);
            free(pedidos);
            return resultado;
        }
    }
    free(pedidos);
    return 0;
}

int eliminarPedido(const char *nombreArchivo, int id)
{
    // Verificar si el pedido existe antes de intentar eliminarlo
    Pedido pedidoEncontrado = buscarPedidoPorId(nombreArchivo, id);
    if (pedidoEncontrado.id == 0)
    {
        printf("Error: No se encontró el pedido con ID %d.\n", id);
        return 0;
    }

    FILE *archivo = fopen(nombreArchivo, "rb");
    FILE *aux = fopen("aux.bin", "wb");

    if (!archivo || !aux)
    {
        perror("Error al abrir los archivos");
        if (archivo) fclose(archivo);
        if (aux) fclose(aux);
        return 0;
    }

    int numPedidos;
    if (fread(&numPedidos, sizeof(int), 1, archivo) != 1)
    {
        printf("Error: No se pudo leer la cantidad de pedidos.\n");
        fclose(archivo);
        fclose(aux);
        remove("aux.bin");
        return 0;
    }

    fwrite(&numPedidos, sizeof(int), 1, aux); // Escribir temporalmente el número de pedidos

    Pedido pedido;
    int pedidosRestantes = 0; // Contador de pedidos válidos

    for (int i = 0; i < numPedidos; i++)
    {
        if (fread(&pedido, sizeof(Pedido), 1, archivo) != 1)
        {
            printf("Error al leer pedido del archivo.\n");
            break;
        }

        if (pedido.id != id)
        {
            fwrite(&pedido, sizeof(Pedido), 1, aux);
            pedidosRestantes++;
        }
    }

    fclose(archivo);
    fclose(aux);

    if (pedidosRestantes == numPedidos) // No se eliminó nada
    {
        remove("aux.bin");
        printf("Error: No se encontró el pedido con ID %d.\n", id);
        return 0;
    }

    // Actualizar el número de pedidos en el archivo auxiliar
    archivo = fopen("aux.bin", "r+b");
    if (archivo)
    {
        fwrite(&pedidosRestantes, sizeof(int), 1, archivo);
        fclose(archivo);
    }

    remove(nombreArchivo);
    rename("aux.bin", nombreArchivo);

    printf("Pedido con ID %d eliminado correctamente.\n", id);
    return 1;
}


 Pedido *buscarPedidosPorMesa(const char *nombreArchivo, int idMesa, int *numPedidos)
{
    int totalPedidos;
    Pedido *pedidos = cargarPedidos(nombreArchivo, &totalPedidos);

    if (!pedidos || totalPedidos == 0)
    {
        printf("No hay pedidos registrados o error al cargar.\n");
        *numPedidos = 0;
        return NULL;
    }

    // Contar cuántos pedidos pertenecen a la mesa indicada
    int contador = 0;
    for (int i = 0; i < totalPedidos; i++)
    {
        if (pedidos[i].mesa.id == idMesa)
        {
            contador++;
        }
    }

    if (contador == 0) // No hay pedidos para la mesa
    {
        printf("No se encontraron pedidos para la mesa %d.\n", idMesa);
        free(pedidos);
        *numPedidos = 0;
        return NULL;
    }

    // Asignar solo el espacio necesario para los pedidos encontrados
    Pedido *pedidosMesa = (Pedido *)malloc(contador * sizeof(Pedido));
    if (!pedidosMesa)
    {
        perror("Error al asignar memoria para los pedidos de la mesa");
        free(pedidos);
        *numPedidos = 0;
        return NULL;
    }

    // Copiar los pedidos correspondientes a la mesa
    int index = 0;
    for (int i = 0; i < totalPedidos; i++)
    {
        if (pedidos[i].mesa.id == idMesa)
        {
            pedidosMesa[index++] = pedidos[i];
        }
    }

    free(pedidos);
    *numPedidos = contador;
    return pedidosMesa;
}

float calcularTotalPedido(const char *nombreArchivoDetalles, int idPedido)
{
    int totalDetalles;
    DetallePedido *detalles = cargarDetallesPedidos(nombreArchivoDetalles, &totalDetalles);

    if (!detalles || totalDetalles == 0)
    {
        printf("No hay detalles de pedidos registrados o error al cargar el archivo.\n");
        return 0.0f;
    }

    float total = 0.0f;
    int encontrado = 0;

    for (int i = 0; i < totalDetalles; i++)
    {
        if (detalles[i].pedido.id == idPedido)
        {
            encontrado = 1;
            total += detalles[i].subTotalProducto; // Usamos el subtotal almacenado en la estructura
        }
    }

    free(detalles);

    if (!encontrado)
    {
        printf("Pedido con ID %d no encontrado.\n", idPedido);
        return 0.0f;
    }

    return total;
}




// PRODUCTOS //

// Función para guardar productos en un archivo binario
int guardarProductos(const char *nombreArchivo, Producto *productos, int numProductos)
{
    FILE *fp = fopen(nombreArchivo, "wb");
    if (fp == NULL)
    {
        perror("Error al abrir el archivo");
        return 0; // Indicar error
    }

    // Escribir un encabezado con el número de productos para facilitar la carga posterior
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
    return 1; // Indicar éxito
}

// Función para cargar productos desde un archivo binario
Producto *cargarProductos(const char *nombreArchivo, int *numProductos)
{
    FILE *fp = fopen(nombreArchivo, "rb");
    if (fp == NULL)
    {
        perror("Error al abrir el archivo");
        return NULL;
    }

    // Leer el número de productos del encabezado
    fread(numProductos, sizeof(int), 1, fp);

    Producto *productos = (Producto *)malloc(*numProductos * sizeof(Producto));
    fread(productos, sizeof(Producto), *numProductos, fp);
    fclose(fp);
    return productos;
}

// Función para buscar un producto por su ID
Producto buscarProductoPorId(const char *nombreArchivo, int id)
{
    int numProductos;
    Producto *productos = cargarProductos(nombreArchivo, &numProductos);

    // Búsqueda secuencial (puedes implementar búsqueda binaria si los productos están ordenados)
    for (int i = 0; i < numProductos; i++)
    {
        if (productos[i].id == id)
        {
            Producto productoEncontrado = productos[i];
            free(productos);
            return productoEncontrado;
        }
    }

    free(productos);
    Producto productoVacio = {0}; // Crear un producto vacío para indicar que no se encontró
    return productoVacio;
}

// Función para agregar un nuevo producto
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
    return 1; // Indicar éxito
}

// Función para modificar un producto existente
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
            return 1; // Modificación exitosa
        }
    }

    free(productos);
    return 0; // Producto no encontrado
}

// Función para eliminar un producto
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
        return 1; // Eliminación exitosa
    }

    free(productos);
    return 0; // Producto no encontrado
}

// Función para buscar productos por categoría
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

//
// Detalle - Pedidos //
//

// Cargar detalles desde archivo binario
DetallePedido *cargarDetallesPedidos(const char *nombreArchivo, int *numDetalles)
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    if (!archivo)
    {
        *numDetalles = 0;
        return NULL;
    }

    // Obtener tamaño del archivo
    fseek(archivo, 0, SEEK_END);
    long tamanoArchivo = ftell(archivo);
    rewind(archivo);

    if (tamanoArchivo == 0)
    {
        *numDetalles = 0;
        fclose(archivo);
        return NULL;
    }

    *numDetalles = tamanoArchivo / sizeof(DetallePedido);
    DetallePedido *detalles = (DetallePedido *)malloc(*numDetalles * sizeof(DetallePedido));

    if (!detalles)
    {
        perror("Error al asignar memoria");
        fclose(archivo);
        return NULL;
    }

    if (fread(detalles, sizeof(DetallePedido), *numDetalles, archivo) != (size_t)*numDetalles)
    {
        perror("Error al leer el archivo");
        free(detalles);
        fclose(archivo);
        return NULL;
    }

    fclose(archivo);
    return detalles;
}

// Guardar detalles en archivo binario
int guardarDetallesPedidos(const char *nombreArchivo, DetallePedido *detalles, int numDetalles)
{
    if (!detalles || numDetalles <= 0)
    {
        printf("No hay detalles para guardar.\n");
        return 0;
    }

    FILE *archivo = fopen(nombreArchivo, "wb");
    if (!archivo)
    {
        perror("Error al abrir el archivo");
        return 0;
    }

    if (fwrite(detalles, sizeof(DetallePedido), numDetalles, archivo) != (size_t)numDetalles)
    {
        perror("Error al escribir los detalles");
        fclose(archivo);
        return 0;
    }

    fclose(archivo);
    return 1;
}

// Buscar detalles por ID de pedido (devuelve una lista de detalles)
DetallePedido *buscarDetallesPorPedido(const char *nombreArchivo, int idPedido, int *numResultados)
{
    int numDetalles;
    DetallePedido *detalles = cargarDetallesPedidos(nombreArchivo, &numDetalles);

    if (!detalles)
    {
        *numResultados = 0;
        return NULL;
    }

    // Contar cuántos detalles coinciden
    *numResultados = 0;
    for (int i = 0; i < numDetalles; i++)
    {
        if (detalles[i].pedido.id == idPedido)
        {
            (*numResultados)++;
        }
    }

    if (*numResultados == 0)
    {
        free(detalles);
        return NULL;
    }

    // Crear un nuevo array con los resultados
    DetallePedido *resultados = (DetallePedido *)malloc(*numResultados * sizeof(DetallePedido));
    if (!resultados)
    {
        perror("Error al asignar memoria");
        free(detalles);
        *numResultados = 0;
        return NULL;
    }

    int index = 0;
    for (int i = 0; i < numDetalles; i++)
    {
        if (detalles[i].pedido.id == idPedido)
        {
            resultados[index++] = detalles[i];
        }
    }

    free(detalles);
    return resultados;
}

// Agregar un nuevo detalle de pedido
int agregarDetallePedido(const char *nombreArchivo, DetallePedido nuevoDetalle)
{
    int numDetalles;
    DetallePedido *detalles = cargarDetallesPedidos(nombreArchivo, &numDetalles);

    DetallePedido *nuevosDetalles = (DetallePedido *)realloc(detalles, (numDetalles + 1) * sizeof(DetallePedido));
    if (!nuevosDetalles)
    {
        perror("Error al asignar memoria");
        free(detalles);
        return 0;
    }

    nuevosDetalles[numDetalles] = nuevoDetalle;
    numDetalles++;

    int resultado = guardarDetallesPedidos(nombreArchivo, nuevosDetalles, numDetalles);
    free(nuevosDetalles);
    return resultado;
}

// Modificar un detalle de pedido existente
int modificarDetallePedido(const char *nombreArchivo, int id, DetallePedido nuevoDetalle)
{
    int numDetalles;
    DetallePedido *detalles = cargarDetallesPedidos(nombreArchivo, &numDetalles);
    if (!detalles)
    {
        return 0;
    }

    int encontrado = 0;
    for (int i = 0; i < numDetalles; i++)
    {
        if (detalles[i].id == id)
        {
            detalles[i] = nuevoDetalle;
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
    {
        free(detalles);
        return 0;
    }

    int resultado = guardarDetallesPedidos(nombreArchivo, detalles, numDetalles);
    free(detalles);
    return resultado;
}

// Eliminar un detalle de pedido
int eliminarDetallePedido(const char *nombreArchivo, int id)
{
    int numDetalles;
    DetallePedido *detalles = cargarDetallesPedidos(nombreArchivo, &numDetalles);
    if (!detalles || numDetalles == 0)
    {
        return 0;
    }

    int encontrado = 0;
    for (int i = 0; i < numDetalles; i++)
    {
        if (detalles[i].id == id)
        {
            encontrado = 1;
            for (int j = i; j < numDetalles - 1; j++)
            {
                detalles[j] = detalles[j + 1];
            }
            break;
        }
    }

    if (!encontrado)
    {
        free(detalles);
        return 0;
    }

    numDetalles--;
    int resultado = guardarDetallesPedidos(nombreArchivo, detalles, numDetalles);
    free(detalles);
    return resultado;
}


//
// USUARIOS
//

// Función para cargar los usuarios desde el archivo binario
int cargarUsuarios(Usuario usuarios[], int maxUsuarios) {
    FILE *file = fopen("Usuarios.dat", "rb");
    if (!file) {
        return 0; // No hay usuarios guardados, retorno 0
    }
    int cantidadUsuarios = fread(usuarios, sizeof(Usuario), maxUsuarios, file);
    fclose(file);
    return cantidadUsuarios;
}

// Función para guardar los usuarios en el archivo binario
void guardarUsuarios(Usuario usuarios[], int cantidadUsuarios) {
    FILE *file = fopen("Usuarios.dat", "wb");
    if (!file) {
        printf("Error al guardar los usuarios.\n");
        return;
    }
    fwrite(usuarios, sizeof(Usuario), cantidadUsuarios, file);
    fclose(file);
}
