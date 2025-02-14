#include "repositories.h"

// Funciones para el struct Mesa

// Función para guardar un arreglo de mesas en un archivo binario
void guardarMesas(const char* nombreArchivo, Mesa* mesas, int numMesas) {
    FILE* archivo = fopen(nombreArchivo, "wb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    fwrite(mesas, sizeof(Mesa), numMesas, archivo);

    fclose(archivo);
}

// Función para cargar un arreglo de mesas desde un archivo binario
Mesa* cargarMesas(const char* nombreArchivo, int* numMesas) {
    FILE* archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        *numMesas = 0; // Inicializar numMesas a 0 en caso de error
        return NULL;
    }

    *numMesas = 0; // Inicializar a 0 antes de leer

    Mesa* mesas = NULL;
    Mesa mesa;
    while (fread(&mesa, sizeof(Mesa), 1, archivo) == 1) {
        (*numMesas)++;
        mesas = (Mesa*)realloc(mesas, (*numMesas) * sizeof(Mesa));
        if (mesas == NULL) {
            perror("Error al reasignar memoria");
            fclose(archivo);
            return NULL;
        }
        mesas[(*numMesas) - 1] = mesa;
    }

    fclose(archivo);
    return mesas;
}

// Función para buscar una mesa por su ID
Mesa buscarMesaPorId(const char* nombreArchivo, int id) {
    Mesa mesaEncontrada;
    int numMesas;
    Mesa* mesas = cargarMesas(nombreArchivo, &numMesas);

    for (int i = 0; i < numMesas; i++) {
        if (mesas[i].id == id) {
            mesaEncontrada = mesas[i];
            break;
        }
    }

    free(mesas); // Liberar la memoria asignada
    return mesaEncontrada;
}

// Función para agregar una nueva mesa
int agregarMesa(const char* nombreArchivo, Mesa nuevaMesa) {
    int numMesas;
    Mesa* mesas = cargarMesas(nombreArchivo, &numMesas);

    // Realizar las validaciones necesarias (por ejemplo, verificar si la mesa ya existe)

    // Incrementar el tamaño del arreglo y agregar la nueva mesa
    numMesas++;
    mesas = (Mesa*)realloc(mesas, numMesas * sizeof(Mesa));
    mesas[numMesas - 1] = nuevaMesa;

    // Guardar el arreglo actualizado en el archivo
    guardarMesas(nombreArchivo, mesas, numMesas);

    free(mesas);
    return 1; // Indicar éxito
}

// Función para buscar pedidos por mesa
Pedido* buscarPedidosPorMesa(const char* nombreArchivo, int idMesa, int* numPedidos) {
    int totalPedidos;
    Pedido* pedidos = cargarPedidos(nombreArchivo, &totalPedidos);

    if (pedidos == NULL) {
        *numPedidos = 0;
        return NULL;
    }

    // Crear un arreglo dinámico para almacenar los pedidos encontrados
    Pedido* pedidosEncontrados = (Pedido*)malloc(totalPedidos * sizeof(Pedido));
    int numPedidosEncontrados = 0;

    for (int i = 0; i < totalPedidos; i++) {
        if (pedidos[i].mesa.id == idMesa) {
            pedidosEncontrados[numPedidosEncontrados++] = pedidos[i];
        }
    }

    // Redimensionar el arreglo de pedidos encontrados al tamaño correcto
    pedidosEncontrados = (Pedido*)realloc(pedidosEncontrados, numPedidosEncontrados * sizeof(Pedido));

    free(pedidos);
    *numPedidos = numPedidosEncontrados;  // Guardamos el número de pedidos encontrados
    return pedidosEncontrados;
}

// Función para calcular el total de un pedido
float calcularTotalPedido(const char* nombreArchivo, int idPedido, int* numDetalles) {
    int totalDetalles;
    DetallePedido* detalles = cargarDetallesPedidos(nombreArchivo, &totalDetalles);

    if (detalles == NULL) {
        *numDetalles = 0;
        return 0.0;
    }

    float total = 0.0;

    for (int i = 0; i < totalDetalles; i++) {
        if (detalles[i].pedido.id == idPedido) {
            total += detalles[i].cantidad * detalles[i].producto.precio;
        }
    }

    free(detalles);
    *numDetalles = totalDetalles;  // Guardamos el número de detalles procesados
    return total;
}


// Cargar pedidos
Pedido* cargarPedidos(const char* nombreArchivo, int* numPedidos) {
    FILE* archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    fseek(archivo, 0, SEEK_END);
    long tamanoArchivo = ftell(archivo);
    rewind(archivo);

    *numPedidos = tamanoArchivo / sizeof(Pedido);
    Pedido* pedidos = (Pedido*)malloc(*numPedidos * sizeof(Pedido));

    fread(pedidos, sizeof(Pedido), *numPedidos, archivo);
    fclose(archivo);
    
    return pedidos;
}

DetallePedido* cargarDetallesPedidos(const char* nombreArchivo, int* numDetalles) {
    FILE* archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    fseek(archivo, 0, SEEK_END);
    long tamanoArchivo = ftell(archivo);
    rewind(archivo);

    *numDetalles = tamanoArchivo / sizeof(DetallePedido);
    DetallePedido* detalles = (DetallePedido*)malloc(*numDetalles * sizeof(DetallePedido));

    fread(detalles, sizeof(DetallePedido), *numDetalles, archivo);
    fclose(archivo);
    
    return detalles;
}

// Función auxiliar para encontrar el índice libre en un arreglo
int encontrarIndiceLibre(const char* nombreArchivo, int tamEstructura) {
    // Para simplificar, asumimos que los registros se almacenan de forma consecutiva
    // y que no hay huecos en el archivo.
    // Si hay huecos, se necesitaría una lógica más compleja para encontrar el siguiente índice libre.

    FILE* archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }

    fseek(archivo, 0, SEEK_END);
    long tamanoArchivo = ftell(archivo);
    fclose(archivo);

    return tamanoArchivo / tamEstructura;
}
