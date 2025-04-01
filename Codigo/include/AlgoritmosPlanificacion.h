#ifndef ALGORITMOS_PLANIFICACION_H
#define ALGORITMOS_PLANIFICACION_H

#include <vector>
#include "Resultado.h"

class AlgoritmosPlanificacion {
public:
    static ResultadoAlgoritmo fcfs(const std::vector<int>& requests, int initialHead);
    static ResultadoAlgoritmo sstf(std::vector<int> requests, int initialHead);
    static ResultadoAlgoritmo scan(std::vector<int> requests, int initialHead, int maxCylinder);
    
private:
    static ResultadoAlgoritmo calcularMetricas(const std::vector<int>& requests, int initialHead, const std::string& nombre);
};

#endif // ALGORITMOS_PLANIFICACION_H