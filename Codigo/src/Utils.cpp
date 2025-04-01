#include "../include/Utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <limits>
#include <vector>
#include <numeric>
#include <cmath>

using namespace std;

vector<string> Utils::dividirEnLineas(const string& str, size_t width) {
    vector<string> lineas;
    istringstream ss(str);
    string palabra;
    string linea;
    
    while (ss >> palabra) {
        if (linea.empty()) {
            linea = palabra;
        } else if (linea.length() + palabra.length() + 1 <= width) {
            linea += " " + palabra;
        } else {
            lineas.push_back(linea);
            linea = palabra;
        }
    }
    
    if (!linea.empty()) {
        lineas.push_back(linea);
    }
    
    return lineas;
}

string Utils::formatearOrden(const vector<int>& orden) {
    if (orden.size() <= 15) {
        stringstream ss;
        for (size_t i = 0; i < orden.size(); ++i) {
            if (i != 0) ss << ", ";
            ss << orden[i];
        }
        return ss.str();
    } else {
        stringstream ss;
        ss << orden[0] << ", " << orden[1] << ", " << orden[2] << ", ..., ";
        ss << orden[orden.size()-3] << ", " 
           << orden[orden.size()-2] << ", " 
           << orden.back();
        return ss.str();
    }
}

string Utils::obtenerFechaHora() {
    time_t ahora = time(nullptr);
    tm* tm_local = localtime(&ahora);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_local);
    return string(buffer);
}

void Utils::guardarEnArchivo(const string& configuracion, 
                            const vector<ResultadoAlgoritmo>& resultados,
                            int initialHead,
                            int numRequests,
                            int maxCylinder) {
    ofstream archivo("registro_simulaciones.txt", ios::app);

    if (archivo.is_open()) {
        double longitudPromedio = 0.0;
        if (!resultados.empty() && !resultados[0].orden.empty()) {
            longitudPromedio = static_cast<double>(resultados[0].tiempoTotal) / numRequests;
        }

        archivo << "========================================\n";
        archivo << "Simulación realizada el: " << obtenerFechaHora() << "\n";
        archivo << "CONFIGURACIÓN:\n";
        archivo << "- Posición inicial del cabezal: " << initialHead << "\n";
        archivo << "- Cantidad de solicitudes: " << numRequests << "\n";
        archivo << "- Máximo cilindro: " << maxCylinder << "\n";
        archivo << "- Longitud promedio: " << fixed << setprecision(2) << longitudPromedio << "\n\n";

        archivo << "DETALLE DE SOLICITUDES:\n";
        archivo << configuracion << "\n\n";

        archivo << "RESULTADOS:\n";
        for (const auto& res : resultados) {
            archivo << "Algoritmo: " << res.nombre << "\n";
            archivo << "Orden de acceso: " << formatearOrden(res.orden) << "\n";
            archivo << "Tiempo total: " << res.tiempoTotal << "\n";
            archivo << "Latencia promedio: " << fixed << setprecision(2) << res.latenciaPromedio << "\n\n";
        }

        auto mejorTiempo = min_element(resultados.begin(), resultados.end(),
            [](const ResultadoAlgoritmo& a, const ResultadoAlgoritmo& b) {
                return a.tiempoTotal < b.tiempoTotal;
            });

        auto mejorLatencia = min_element(resultados.begin(), resultados.end(),
            [](const ResultadoAlgoritmo& a, const ResultadoAlgoritmo& b) {
                return a.latenciaPromedio < b.latenciaPromedio;
            });

        archivo << "CONCLUSIONES:\n";
        archivo << "-> Mejor algoritmo en tiempo: " 
               << mejorTiempo->nombre << " (" << mejorTiempo->tiempoTotal << ")\n";
        archivo << "-> Mejor algoritmo en latencia: " 
               << mejorLatencia->nombre << " (" << fixed << setprecision(2)
               << mejorLatencia->latenciaPromedio << ")\n";

        archivo << "========================================\n\n";
        archivo.close();
        cout << VERDE << "Resultados guardados en registro_simulaciones.txt\n" << RESET;
    } else {
        cout << ROJO << "Error al abrir el archivo para guardar resultados.\n" << RESET;
    }
}

void Utils::mostrarResultados(const vector<ResultadoAlgoritmo>& resultados, 
                            int initialHead, 
                            int maxCylinder) {
    const int anchoAlg = 12;
    const int anchoOrden = 36;
    const int anchoTiempo = 14;
    const int anchoLatencia = 18;
    
    cout << AMARILLO << "\nRESULTADOS COMPARATIVOS\n" << RESET;
    cout << "Configuración usada:\n";
    cout << "• Posición inicial: " << initialHead << "\n";
    cout << "• Cilindros máximos: " << maxCylinder << "\n";
    
    if (!resultados.empty() && !resultados[0].orden.empty()) {
        double longitudPromedio = static_cast<double>(resultados[0].tiempoTotal) / resultados[0].orden.size();
        cout << "• Longitud promedio de solicitudes: " << fixed << setprecision(2) << longitudPromedio << "\n";
    }
    cout << "\n";
    
    cout << VERDE << "+--------------+--------------------------------------+--------------+------------------+\n" << RESET;
    cout << VERDE << "| " << left << setw(anchoAlg) << "Algoritmo" << " | " 
         << setw(anchoOrden) << "Orden de Acceso" << " | " 
         << setw(anchoTiempo) << "Tiempo Total" << " | " 
         << setw(anchoLatencia) << "Latencia Promedio" << " |\n";
    cout << VERDE << "+--------------+--------------------------------------+--------------+------------------+\n" << RESET;
    
    for (const auto& res : resultados) {
        string color;
        if (res.nombre == "FCFS") color = CYAN;
        else if (res.nombre == "SSTF") color = MAGENTA;
        else color = AZUL;
        
        string ordenStr = formatearOrden(res.orden);
        vector<string> lineasOrden = dividirEnLineas(ordenStr, anchoOrden);
        
        cout << VERDE << "| " << RESET << color << left << setw(anchoAlg) << res.nombre << RESET << VERDE << " | " << RESET;
        cout << left << setw(anchoOrden) << (lineasOrden.empty() ? "" : lineasOrden[0]) << VERDE << " | " << RESET;
        cout << right << setw(anchoTiempo) << res.tiempoTotal << VERDE << " | " << RESET;
        cout << right << setw(anchoLatencia) << fixed << setprecision(2) << res.latenciaPromedio << VERDE << " |\n" << RESET;
        
        for (size_t i = 1; i < lineasOrden.size(); ++i) {
            cout << VERDE << "| " << RESET << setw(anchoAlg) << "" << VERDE << " | " << RESET;
            cout << left << setw(anchoOrden) << lineasOrden[i] << VERDE << " | " << RESET;
            cout << setw(anchoTiempo) << "" << VERDE << " | " << RESET;
            cout << setw(anchoLatencia) << "" << VERDE << " |\n" << RESET;
        }
        
        cout << VERDE << "+--------------+--------------------------------------+--------------+------------------+\n" << RESET;
    }
}