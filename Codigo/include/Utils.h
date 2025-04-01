#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <iomanip>
#include "Resultado.h"

// Códigos ANSI para colores
#define RESET   "\033[0m"
#define ROJO    "\033[31m"
#define VERDE   "\033[32m"
#define AMARILLO "\033[33m"
#define AZUL    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

namespace Utils {
    std::vector<std::string> dividirEnLineas(const std::string& str, size_t width);
    std::string formatearOrden(const std::vector<int>& orden);
    std::string obtenerFechaHora();
    void guardarEnArchivo(const std::string& configuracion, 
                         const std::vector<ResultadoAlgoritmo>& resultados,
                         int initialHead,
                         int numRequests,
                         int maxCylinder);
    void mostrarResultados(const std::vector<ResultadoAlgoritmo>& resultados, 
                         int initialHead, 
                         int maxCylinder);
}

#endif // UTILS_H