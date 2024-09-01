#include "Client.hpp"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>

Client::Client(const std::string& server_ip, int server_port)
    : server_ip(server_ip), server_port(server_port), sock(-1) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Error al crear el socket." << std::endl;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);
}

bool Client::connectToServer() {
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error al conectar al servidor." << std::endl;
        return false;
    }
    std::cout << "Conectado al servidor " << server_ip << ":" << server_port << std::endl;
    return true;
}

void Client::sendMessage(const std::string& message) {
    send(sock, message.c_str(), message.size(), 0);
}

std::string Client::receiveMessage() {
    char buffer[1024] = {0};
    ssize_t len = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (len > 0) {
        return std::string(buffer, len);
    } else {
        return std::string();
    }
}
