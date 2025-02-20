#include "mesa-repositorio.h"

// Funcion para guardar un arreglo de mesas en un archivo binario
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

// Funcion para cargar un arreglo de mesas desde un archivo binario
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

// Funcion para buscar una mesa por su ID
Mesa buscarMesaPorId(const char *nombreArchivo, int id)
{
    Mesa mesaEncontrada = {0};
    int numMesas;
    Mesa *mesas = cargarMesas(nombreArchivo, &numMesas);
    int i = 0;
    if (mesas)
    {
        for (int i = 0; i < numMesas; i++)
        {
            if (mesas[i].id == id)
            {
                mesaEncontrada = mesas[i];
                break;
            }
        }
        free(mesas); // Liberar la memoria asignada
    }
    return mesaEncontrada;
}

// Funcion para agregar una nueva mesa
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
    return 1; // Indicar exito
}


// Funcion auxiliar para encontrar el indice libre en un arreglo
int encontrarIndiceLibre(const char *nombreArchivo, int tamEstructura)
{
    // Para simplificar, asumimos que los registros se almacenan de forma consecutiva
    // y que no hay huecos en el archivo.
    // Si hay huecos, se necesitaria una logica mas compleja para encontrar el siguiente indice libre.

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