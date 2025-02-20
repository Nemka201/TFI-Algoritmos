#include "pedido-repositorio.h"
#include "detalle-repositorio.h"

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
int obtenerUltimoIdPedido(const char *nombreArchivo)
{
    FILE *fp = fopen(nombreArchivo, "rb");
    if (!fp)
    {
        return 0; // Si el archivo no existe, empezamos desde 0
    }

    int numPedidos;
    if (fread(&numPedidos, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return 0; // Si no se puede leer el numero de pedidos, empezamos desde 0
    }

    if (numPedidos <= 0)
    {
        fclose(fp);
        return 0; // No hay pedidos registrados
    }

    // Ir al ultimo pedido
    fseek(fp, sizeof(int) + (numPedidos - 1) * sizeof(Pedido), SEEK_SET);

    Pedido ultimoPedido;
    if (fread(&ultimoPedido, sizeof(Pedido), 1, fp) != 1)
    {
        fclose(fp);
        return 0; // Si hay un error, asumimos que no hay pedidos validos
    }

    fclose(fp);
    return ultimoPedido.id;
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

    // Verifica que se lea correctamente el numero de pedidos
    if (fread(numPedidos, sizeof(int), 1, fp) != 1)  
    {
        perror("Error al leer la cantidad de pedidos o archivo vacio");
        fclose(fp);
        *numPedidos = 0;
        return NULL;
    }

    // Si el numero de pedidos es invalido, evitar malloc con tamaño 0 o negativo
    if (*numPedidos <= 0)
    {
        // printf("Advertencia: Archivo sin pedidos.\n");
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

    // Reservar mas memoria para el nuevo pedido
    Pedido *nuevosPedidos = (Pedido *)realloc(pedidos, (numPedidos + 1) * sizeof(Pedido));
    if (!nuevosPedidos)
    {
        perror("Error al realocar memoria... ");
        free(pedidos);  // Solo liberar si realloc fallo
        return 0;
    }

    // Agregar el nuevo pedido al array
    nuevosPedidos[numPedidos] = nuevoPedido;
    numPedidos++;

    // Guardar los pedidos en el archivo
    int resultado = guardarPedidos(nombreArchivo, nuevosPedidos, numPedidos);
    free(nuevosPedidos); // Liberar memoria despues de guardar
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
        printf("Error: No se encontro el pedido con ID %d.\n", id);
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

    fwrite(&numPedidos, sizeof(int), 1, aux); // Escribir temporalmente el numero de pedidos

    Pedido pedido;
    int pedidosRestantes = 0; // Contador de pedidos validos

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

    if (pedidosRestantes == numPedidos) // No se elimino nada
    {
        remove("aux.bin");
        printf("Error: No se encontro el pedido con ID %d.\n", id);
        return 0;
    }

    // Actualizar el numero de pedidos en el archivo auxiliar
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
        printf("No hay pedidos registrados.\n");
        *numPedidos = 0;
        return NULL;
    }

    // Contar cuantos pedidos pertenecen a la mesa indicada
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
        printf("No hay detalles de pedidos registrados.\n");
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
        // printf("Pedido con ID %d no encontrado.\n", idPedido);
        return 0.0f;
    }

    return total;
}
