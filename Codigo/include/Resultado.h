#ifndef RESULTADO_H
#define RESULTADO_H

#include <vector>
#include <string>

struct ResultadoAlgoritmo {
    std::string nombre;
    std::vector<int> orden;
    int tiempoTotal;
    double latenciaPromedio;
};

#endif // RESULTADO_H