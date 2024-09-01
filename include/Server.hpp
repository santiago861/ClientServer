#include <memory>
#include <netinet/in.h>
#include <string>

class Server {
public:
    Server(int port);
    void start();
    
private:
    int server_socket;
    int port;
    void handleClient(int client_socket);
    int acceptConnection();
    std::string receiveMessage(int client_socket);
    void sendMessage(int client_socket, const std::string& message);
};
