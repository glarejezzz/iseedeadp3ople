#include "SocketWrapper.h"
#include <iostream>
#include <string>

int main() {
    const int PORT = 12345;
    const std::string HOST = "127.0.0.1";
    
    try {
        WSAInitializer wsa;
        ClientSocket client;
        client.connect(HOST, PORT);
        std::cout << "[Client] Connected to KV-Store server at " << HOST << ":" << PORT << std::endl;
        std::cout << "Commands: SET <key> <value>, GET <key>, DEL <key>, QUIT" << std::endl;

        while (true) {
            std::cout << "> ";
            std::string command;
            std::getline(std::cin, command);

            if (command.empty()) continue;

            client.send(command + "\n");
            std::string response = client.receive(4096);

            if (response == "NOT_FOUND\n") {
                std::cout << "[MISS] " << response;
            } else if (response == "QUIT\n") {
                std::cout << "[Client] Disconnecting..." << std::endl;
                break;
            } else {
                std::cout << response;
            }
        }
    } catch (const NetworkException& e) {
        std::cerr << "[Client] Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}