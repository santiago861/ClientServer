#include <iostream>
#include <nlohmann/json.hpp>
#include "Client.hpp"

int main() {
    try {
        Client client("127.0.0.1", 8080);
        if (!client.connectToServer()) {
            return 1;
        }

        while (true) {
            std::string user_input;
            std::cout << "Ingrese el mensaje para enviar al servidor (o 'salir' para desconectar): ";
            std::getline(std::cin, user_input);

            if (user_input == "salir") {
                break;
            }

            nlohmann::json message;
            message["content"] = user_input;
            message["type"] = "message";

            client.sendMessage(message.dump());

            std::string response_str = client.receiveMessage();
            if (!response_str.empty()) {
                nlohmann::json response = nlohmann::json::parse(response_str);
                std::cout << "Respuesta del servidor: " << response.dump(4) << std::endl;
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error en el cliente: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
