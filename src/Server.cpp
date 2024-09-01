#include "Server.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <cstring>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Server::Server(int port) : port(port) {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error al crear el socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error en bind." << std::endl;
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) < 0) {
        std::cerr << "Error en listen." << std::endl;
        close(server_socket);
        exit(EXIT_FAILURE);
    }
}

void Server::start() {
    std::cout << "Servidor iniciado en el puerto " << port << std::endl;
    while (true) {
        int client_socket = acceptConnection();
        if (client_socket != -1) {
            std::thread(&Server::handleClient, this, client_socket).detach();
        }
    }
}

void Server::handleClient(int client_socket) {
    std::cout << "Cliente conectado: " << client_socket << std::endl;

    while (true) {
        std::string message = receiveMessage(client_socket);
        if (message.empty()) {
            std::cout << "Cliente desconectado o error de lectura." << std::endl;
            break;
        }

        std::cout << "Mensaje recibido del cliente: " << message << std::endl;
        
        // Procesar el mensaje recibido y generar la respuesta
        std::string response = "{\"received\":" + message + ",\"status\":\"success\"}";
        sendMessage(client_socket, response);
    }

    close(client_socket);
}

int Server::acceptConnection() {
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_len);

    if (client_socket < 0) {
        std::cerr << "Error al aceptar la conexión." << std::endl;
        return -1;
    }

    std::cout << "Cliente conectado: " << inet_ntoa(client_address.sin_addr) << std::endl;
    return client_socket;
}

std::string Server::receiveMessage(int client_socket) {
    char buffer[1024] = {0};
    ssize_t len = recv(client_socket, buffer, 1024, 0);
    if (len <= 0) {
        return "";
    }
    return std::string(buffer, len);
}

void Server::sendMessage(int client_socket, const std::string& message) {
    send(client_socket, message.c_str(), message.size(), 0);
}
