#include "SocketWrapper.h"
#include <iostream>
#include <chrono>

void Client::run(const std::string& ip_address, int port) {
    WSAInitializer wsa;
    ClientSocket client;
    
    std::cout << "[Client] Connecting to " << ip_address << ":" << port << "..." << std::endl;
    client.connect(ip_address, port);
    std::cout << "[Client] Connected to calculator server!" << std::endl;
    std::cout << "Commands: CALC <a> <op> <b>" << std::endl;
    std::cout << "Example: CALC 10.5 * 2" << std::endl;
    std::cout << "Type 'exit' to quit" << std::endl;
    
    while (true) {
        std::cout << "> ";
        std::string expression;
        std::getline(std::cin, expression);
        
        if (expression == "exit" || expression == "quit") {
            break;
        }
        
        if (expression.empty()) continue;
        
        auto start = std::chrono::steady_clock::now();
        
        try {
            client.send(expression + "\n");
            std::string response = client.receive_line();
            auto end = std::chrono::steady_clock::now();
            auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            
            if (response.find("ERROR:") == 0) {
                std::cout << "[FAIL] " << response << " (Delay: " << delay << " ms)" << std::endl;
            } else {
                std::cout << response << " (Delay: " << delay << " ms)" << std::endl;
            }
            
        } catch (const NetworkException& e) {
            std::cout << "[Client] Network error: " << e.what() << std::endl;
            break;
        } catch (const std::exception& e) {
            std::cout << "[Client] Error: " << e.what() << std::endl;
            break;
        }
    }
}