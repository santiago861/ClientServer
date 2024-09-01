#include <iostream>
#include <thread>
#include <chrono>
#include "Server.hpp"

int main() {
    try {
        Server server(8080);
        server.start();

        // Mantener el servidor activo
        std::this_thread::sleep_for(std::chrono::minutes(10));
    } catch (const std::exception& e) {
        std::cerr << "Error en el servidor: " << e.what() << std::endl;
    }

    return 0;
}
