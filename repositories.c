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
        return NULL;
    }

    fseek(archivo, 0, SEEK_END);
    long tamanoArchivo = ftell(archivo);
    rewind(archivo);

    *numMesas = tamanoArchivo / sizeof(Mesa);
    Mesa* mesas = (Mesa*)malloc(*numMesas * sizeof(Mesa));

    fread(mesas, sizeof(Mesa), *numMesas, archivo);

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
    Pedido* pedidos = cargarPedidos(nombreArchivo, &numPedidos);
    
    // Crear un arreglo dinámico para almacenar los pedidos encontrados
    Pedido* pedidosEncontrados = (Pedido*)malloc((int)numPedidos * sizeof(Pedido));
    int numPedidosEncontrados = 0;

    for (int i = 0; i < numPedidos; i++) {
        if (pedidos[i].mesa.id == idMesa) {
            pedidosEncontrados[numPedidosEncontrados++] = pedidos[i];
        }
    }

    // Redimensionar el arreglo de pedidos encontrados al tamaño correcto
    pedidosEncontrados = (Pedido*)realloc(pedidosEncontrados, numPedidosEncontrados * sizeof(Pedido));

    free(pedidos);
    return pedidosEncontrados;
}

// Función para calcular el total de un pedido
float calcularTotalPedido(const char* nombreArchivo, int idPedido, int numDetalles) {
    DetallePedido* detalles = cargarDetallesPedidos(nombreArchivo, &numDetalles);
    float total = 0.0;

    for (int i = 0; i < numDetalles; i++) {
        if (detalles[i].pedido.id == idPedido) {
            total += detalles[i].cantidad * detalles[i].producto.precio;
        }
    }

    free(detalles);
    return total;
}

// ... (resto de las funciones para Productos y Detalles de Pedidos)

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
