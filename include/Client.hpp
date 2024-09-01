#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <netinet/in.h>
#include <nlohmann/json.hpp>

class Client {
public:
    Client(const std::string& server_ip, int server_port);
    bool connectToServer();
    void sendMessage(const std::string& message);
    std::string receiveMessage();

private:
    std::string server_ip;
    int server_port;
    int sock;
    struct sockaddr_in server_addr;
};

#endif // CLIENT_HPP
