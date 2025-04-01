#ifndef DISCO_SIMULADOR_H
#define DISCO_SIMULADOR_H

#include <vector>
#include <map>
#include <string>
#include <utility>
#include "Resultado.h"

class DiscoSimulador {
public:
    DiscoSimulador();
    bool ejecutarSimulacion();
    void mostrarConfiguracion() const;
    
private:
    int initialHead;
    int maxCylinder;
    std::vector<int> requests;
    std::map<std::string, std::vector<int>> dataSets;
    
    void generarDataSetAleatorio(std::vector<int>& requests, int& initialHead, int maxCylinder, int numRequests);
    void configurarCasoPersonalizado();
    void configurarCasoAleatorioCompleto();
    void mostrarAnalisisComparativo(const std::vector<ResultadoAlgoritmo>& resultados); // Nueva función
};

#endif // DISCO_SIMULADOR_H