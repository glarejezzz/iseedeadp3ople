#include "SocketWrapper.h"
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>

std::atomic<bool> running{true};

void receive_messages(ClientSocket& client) {
    while (running) {
        try {
            std::string message = client.receive(4096);
            if (message.empty()) {
                std::cout << "\n[SYSTEM] Disconnected from server" << std::endl;
                running = false;
                break;
            }
            
            while (!message.empty() && (message.back() == '\n' || message.back() == '\r')) {
                message.pop_back();
            }
            
            if (message == "PING") {
                client.send("PONG\n");
            } else if (message.find("[SYSTEM]") == 0) {
                std::cout << "\033[33m" << message << "\033[0m" << std::endl;
            } else {
                std::cout << "\033[36m>> " << message << "\033[0m" << std::endl;
            }
            
            std::cout << "> ";
            std::cout.flush();
        } catch (const NetworkException& e) {
            std::cout << "\n[ERROR] Connection lost: " << e.what() << std::endl;
            running = false;
            break;
        }
    }
}

int main() {
    const int PORT = 12345;
    const std::string HOST = "127.0.0.1";
    
    try {
        WSAInitializer wsa;
        ClientSocket client;
        client.connect(HOST, PORT);
        
        std::cout << "[Client] Connected to Broadcast Chat Server at " << HOST << ":" << PORT << std::endl;
        
        std::string nickname;
        std::cout << "Enter your nickname: ";
        std::getline(std::cin, nickname);
        
        if (nickname.empty()) {
            std::cerr << "[ERROR] Nickname cannot be empty" << std::endl;
            return 1;
        }
        
        client.send("NICK " + nickname + "\n");
        
        std::string response = client.receive(4096);
        while (!response.empty() && (response.back() == '\n' || response.back() == '\r')) {
            response.pop_back();
        }
        
        if (response == "WELCOME") {
            std::cout << "\033[32m[SUCCESS] Connected to chat as '" << nickname << "'\033[0m" << std::endl;
            std::cout << "Type your messages and press Enter to send" << std::endl;
            std::cout << "Type '/quit' to exit" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
        } else if (response.find("ERROR: nickname_taken") == 0) {
            std::cerr << "\033[31m[ERROR] Nickname already taken. Please restart with different nickname.\033[0m" << std::endl;
            return 1;
        } else {
            std::cerr << "\033[31m[ERROR] " << response << "\033[0m" << std::endl;
            return 1;
        }
        
        std::thread receiver(receive_messages, std::ref(client));
        
        while (running) {
            std::cout << "> ";
            std::string message;
            std::getline(std::cin, message);
            
            if (!running) break;
            
            if (message == "/quit") {
                running = false;
                break;
            }
            
            if (message.empty()) {
                continue;
            }
            
            try {
                client.send(message + "\n");
            } catch (const NetworkException& e) {
                std::cerr << "\n[ERROR] Failed to send message: " << e.what() << std::endl;
                running = false;
                break;
            }
        }
        
        try {
            client.close();
        } catch (...) {}
        
        if (receiver.joinable()) {
            receiver.join();
        }
        
        std::cout << "[Client] Disconnected" << std::endl;
        
    } catch (const NetworkException& e) {
        std::cerr << "[Client] Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}