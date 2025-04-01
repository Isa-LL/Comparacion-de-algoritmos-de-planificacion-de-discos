#include "../include/AlgoritmosPlanificacion.h"
#include <algorithm>
#include <cmath>
#include <numeric>

ResultadoAlgoritmo AlgoritmosPlanificacion::calcularMetricas(const std::vector<int>& requests, int initialHead, const std::string& nombre) {
    ResultadoAlgoritmo resultado;
    resultado.nombre = nombre;
    resultado.orden = requests;
    
    int totalSeekTime = 0;
    int currentHead = initialHead;
    
    for (int request : requests) {
        totalSeekTime += abs(request - currentHead);
        currentHead = request;
    }
    
    resultado.tiempoTotal = totalSeekTime;
    resultado.latenciaPromedio = static_cast<double>(totalSeekTime) / requests.size();
    
    return resultado;
}

ResultadoAlgoritmo AlgoritmosPlanificacion::fcfs(const std::vector<int>& requests, int initialHead) {
    return calcularMetricas(requests, initialHead, "FCFS");
}

ResultadoAlgoritmo AlgoritmosPlanificacion::sstf(std::vector<int> requests, int initialHead) {
    std::vector<int> orden;
    int currentHead = initialHead;
    
    while (!requests.empty()) {
        auto closest = std::min_element(requests.begin(), requests.end(), 
            [currentHead](int a, int b) {
                return abs(a - currentHead) < abs(b - currentHead);
            });
        
        orden.push_back(*closest);
        currentHead = *closest;
        requests.erase(closest);
    }
    
    return calcularMetricas(orden, initialHead, "SSTF");
}

ResultadoAlgoritmo AlgoritmosPlanificacion::scan(std::vector<int> requests, int initialHead, int maxCylinder) {
    std::vector<int> orden;
    int currentHead = initialHead;
    bool movingRight = true;
    
    std::sort(requests.begin(), requests.end());
    
    while (!requests.empty()) {
        if (movingRight) {
            for (int i = currentHead; i <= maxCylinder; ++i) {
                auto it = std::find(requests.begin(), requests.end(), i);
                if (it != requests.end()){
                    orden.push_back(i);
                    currentHead = i;
                    requests.erase(it);
                }
            }
            movingRight = false;
        } else {
            for (int i = currentHead; i >= 0; --i) {
                auto it = std::find(requests.begin(), requests.end(), i);
                if (it != requests.end()) {
                    orden.push_back(i);
                    currentHead = i;
                    requests.erase(it);
                }
            }
            movingRight = true;
        }
    }
    
    return calcularMetricas(orden, initialHead, "SCAN");
}