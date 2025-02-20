#include "detalle-repositorio.h"

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

    // Contar cuantos detalles coinciden
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

int obtenerUltimoIdDetallePedido(const char *nombreArchivo)
{
    FILE *fp = fopen(nombreArchivo, "rb");
    if (!fp)
    {
        return -1; // Si el archivo no existe, comenzamos desde 0
    }

    int numDetalles;
    if (fread(&numDetalles, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return -1;
    }

    if (numDetalles <= 0)
    {
        fclose(fp);
        return -1;
    }

    fseek(fp, sizeof(int) + (numDetalles - 1) * sizeof(DetallePedido), SEEK_SET);

    DetallePedido ultimoDetalle;
    if (fread(&ultimoDetalle, sizeof(DetallePedido), 1, fp) != 1)
    {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return ultimoDetalle.id;
}