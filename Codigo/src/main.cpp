#include "../include/DiscoSimulador.h"
#include <iostream>  
#include "../include/Utils.h"  

int main() {
    DiscoSimulador simulador;
    bool continuar = true;
    
    while (continuar) {
        continuar = simulador.ejecutarSimulacion();
        
        if (continuar) {
            std::cout << "\n";
            std::cout << AMARILLO << "====================================================\n" << RESET;
            std::cout << AMARILLO << "         PREPARANDO NUEVA SIMULACIÓN         \n" << RESET;
            std::cout << AMARILLO << "====================================================\n\n" << RESET;
        }
    }
    
    std::cout << VERDE << "Todos los resultados se han guardado en 'registro_simulaciones.txt'\n" << RESET;
    
    return 0;
}