#include "../include/DiscoSimulador.h"
#include "../include/AlgoritmosPlanificacion.h"
#include "../include/Utils.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <limits>
#include <cctype>
#include <iomanip>
#include <string>
#include <utility>

using namespace std;

DiscoSimulador::DiscoSimulador() : initialHead(53), maxCylinder(20000) {
    dataSets = {
        {"Caso Básico (8 solicitudes)", {98, 183, 37, 122, 14, 124, 65, 67}},
        {"Caso Medio (8 solicitudes)", {45, 23, 89, 12, 150, 34, 67, 90}},
        {"Caso Complejo (15 solicitudes extremas)", {0, 199, 5, 195, 10, 190, 15, 185, 20, 180, 25, 175, 30, 170, 35}},
        {"Generación Aleatoria Personalizada", {}},
        {"Generación Aleatoria Completa (cantidad aleatoria)", {}}
    };
}

void DiscoSimulador::mostrarConfiguracion() const {
    cout << AMARILLO << "\nCONFIGURACIÓN INICIAL:\n" << RESET;
    cout << "➤ Posición inicial del cabezal: " << initialHead << "\n";
    cout << "➤ Máximo cilindro: " << maxCylinder << "\n";
    cout << "➤ Cilindros solicitados (" << requests.size() << "): ";
    
    if(requests.size() <= 15) {
        for(size_t i = 0; i < requests.size(); ++i) {
            if(i != 0) cout << ", ";
            cout << requests[i];
        }
    } else {
        cout << requests[0] << ", " << requests[1] << ", " << requests[2] << ", ..., ";
        cout << requests[requests.size()-3] << ", " 
             << requests[requests.size()-2] << ", " 
             << requests.back();
    }
    cout << "\n" << AMARILLO << "====================================================\n" << RESET;
}

void DiscoSimulador::generarDataSetAleatorio(vector<int>& requests, int& initialHead, int maxCylinder, int numRequests) {
    random_device rd;
    mt19937 gen(rd());
    
    requests.resize(numRequests);
    uniform_int_distribution<> req_dist(0, maxCylinder);
    
    for(int i = 0; i < numRequests; ++i) {
        requests[i] = req_dist(gen);
    }
    
    initialHead = req_dist(gen);
}

void DiscoSimulador::configurarCasoPersonalizado() {
    int numRequests;
    cout << "\nIngrese la cantidad de solicitudes de cilindros a generar: ";
    cin >> numRequests;
    
    while(numRequests <= 0) {
        cout << ROJO << "Error: Debe ingresar un número positivo.\n" << RESET;
        cout << "Ingrese la cantidad de solicitudes de cilindros a generar: ";
        cin >> numRequests;
    }
    
    generarDataSetAleatorio(requests, initialHead, maxCylinder, numRequests);
    
    cout << VERDE << "\nSe ha generado automáticamente un conjunto de datos con:\n";
    cout << "- " << numRequests << " solicitudes aleatorias\n";
}

void DiscoSimulador::configurarCasoAleatorioCompleto() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> num_req_dist(5, 50);
    
    int numRequests = num_req_dist(gen);
    generarDataSetAleatorio(requests, initialHead, maxCylinder, numRequests);
    
    cout << VERDE << "\nSe ha generado automáticamente un conjunto de datos con:\n";
    cout << "- " << numRequests << " solicitudes aleatorias (cantidad aleatoria)\n";
}

void DiscoSimulador::mostrarAnalisisComparativo(const vector<ResultadoAlgoritmo>& resultados) {
    // Obtener tiempos de cada algoritmo
    int tiempoFCFS = 0, tiempoSSTF = 0, tiempoSCAN = 0;
    for (const auto& res : resultados) {
        if (res.nombre == "FCFS") tiempoFCFS = res.tiempoTotal;
        else if (res.nombre == "SSTF") tiempoSSTF = res.tiempoTotal;
        else if (res.nombre == "SCAN") tiempoSCAN = res.tiempoTotal;
    }

    cout << MAGENTA << "\nANÁLISIS COMPARATIVO DE EFICIENCIA:\n" << RESET;
    
    // SSTF
    cout << AZUL << "➢ SSTF:\n" << RESET;
    cout << "   - " << 100*(tiempoFCFS-tiempoSSTF)/tiempoFCFS << "% más rápido que FCFS\n";
    cout << "   - " << 100*(tiempoSCAN-tiempoSSTF)/tiempoSCAN << "% más rápido que SCAN\n";
    
    // SCAN
    cout << AZUL << "➢ SCAN:\n" << RESET;
    cout << "   - " << 100*(tiempoFCFS-tiempoSCAN)/tiempoFCFS << "% más rápido que FCFS\n";
    cout << "   - " << 100*(tiempoSSTF-tiempoSCAN)/tiempoSSTF << "% más lento que SSTF\n";
}

bool DiscoSimulador::ejecutarSimulacion() {
    string configStr;
    
    cout << CYAN << "\nSIMULADOR DE ALGORITMOS DE PLANIFICACIÓN DE DISCOS\n" << RESET;
    cout << AMARILLO << "====================================================\n" << RESET;
    cout << "Seleccione el conjunto de datos a utilizar:\n";
    
    int i = 1;
    for(const auto& ds : dataSets) {
        cout << i++ << ". " << ds.first;
        if(!ds.second.empty()) {
            cout << " (predefinido)";
        }
        cout << "\n";
    }
    cout << "Ingrese su opción (1-" << dataSets.size() << "): ";
    
    int opcion;
    cin >> opcion;
    if(opcion < 1 || opcion > static_cast<int>(dataSets.size())) {
        cout << ROJO << "Opción inválida. Usando Caso Básico por defecto.\n" << RESET;
        opcion = 1;
    }
    
    auto it = dataSets.begin();
    advance(it, opcion-1);
    
    if(it->first == "Generación Aleatoria Personalizada") {
        configurarCasoPersonalizado();
        configStr = "Configuración generada aleatoriamente:\n";
        configStr += "- " + to_string(requests.size()) + " solicitudes aleatorias\n";
    } 
    else if(it->first == "Generación Aleatoria Completa (cantidad aleatoria)") {
        configurarCasoAleatorioCompleto();
        configStr = "Configuración generada completamente aleatoria:\n";
        configStr += "- " + to_string(requests.size()) + " solicitudes aleatorias\n";
    }
    else {
        requests = it->second;
        configStr = "Configuración inicial:\n";
        configStr += "- Caso utilizado: " + it->first + "\n";
    }
    
    mostrarConfiguracion();
    
    vector<ResultadoAlgoritmo> resultados;
    resultados.push_back(AlgoritmosPlanificacion::fcfs(requests, initialHead));
    resultados.push_back(AlgoritmosPlanificacion::sstf(requests, initialHead));
    resultados.push_back(AlgoritmosPlanificacion::scan(requests, initialHead, maxCylinder));
    
    Utils::mostrarResultados(resultados, initialHead, maxCylinder);
    Utils::guardarEnArchivo(configStr, resultados, initialHead, requests.size(), maxCylinder);
    
    mostrarAnalisisComparativo(resultados);
    
    cout << MAGENTA << "\nCONCLUSIONES FINALES:\n" << RESET;
    auto mejorTiempo = min_element(resultados.begin(), resultados.end(),
        [](const ResultadoAlgoritmo& a, const ResultadoAlgoritmo& b) {
            return a.tiempoTotal < b.tiempoTotal;
        });
    
    auto mejorLatencia = min_element(resultados.begin(), resultados.end(),
        [](const ResultadoAlgoritmo& a, const ResultadoAlgoritmo& b) {
            return a.latenciaPromedio < b.latenciaPromedio;
        });
    
    cout << VERDE << "➤ Algoritmo más eficiente en tiempo: " 
         << mejorTiempo->nombre << " (" << mejorTiempo->tiempoTotal 
         << " unidades de tiempo)\n" << RESET;
    
    cout << AZUL << "➤ Algoritmo con mejor latencia promedio: " 
         << mejorLatencia->nombre << " (" << fixed << setprecision(2)
         << mejorLatencia->latenciaPromedio << " unidades por solicitud)\n" << RESET;
    
    cout << AMARILLO << "\n¿Desea realizar otra simulación? (S/N): " << RESET;
    char respuesta;
    cin >> respuesta;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    return (toupper(respuesta) == 'S');
}