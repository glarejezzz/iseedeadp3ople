#include "SocketWrapper.h"
#include <iostream>
#include <string>
#include <iomanip>

void print_table_header() {
    std::cout << "\n+----------------------+----------+--------+" << std::endl;
    std::cout << "| Process              | PID      | CPU%    |" << std::endl;
    std::cout << "+----------------------+----------+--------+" << std::endl;
}

void print_table_row(const std::string& process_name, const std::string& pid, const std::string& cpu) {
    std::cout << "| " << std::left << std::setw(20) << process_name 
              << "| " << std::setw(8) << pid 
              << "| " << std::setw(6) << cpu << " |" << std::endl;
    std::cout << "+----------------------+----------+--------+" << std::endl;
}

int main() {
    const int PORT = 12345;
    const std::string HOST = "127.0.0.1";
    
    try {
        WSAInitializer wsa;
        ClientSocket client;
        client.connect(HOST, PORT);
        std::cout << "[Client] Connected to Process Monitor Server at " << HOST << ":" << PORT << std::endl;
        std::cout << "Commands: STATUS <process_name>.exe" << std::endl;
        std::cout << "Example: STATUS notepad.exe" << std::endl;
        std::cout << "Type 'QUIT' to exit\n" << std::endl;

        while (true) {
            std::cout << "> ";
            std::string command;
            std::getline(std::cin, command);

            if (command.empty()) continue;
            
            if (command == "QUIT" || command == "quit") {
                client.send("QUIT\n");
                std::cout << "[Client] Disconnecting..." << std::endl;
                break;
            }

            client.send(command + "\n");
            std::string response = client.receive(4096);

            if (response.find("RUNNING") != std::string::npos) {
                // Парсим ответ: RUNNING PID:1234 CPU:12.5%
                size_t pid_pos = response.find("PID:");
                size_t cpu_pos = response.find("CPU:");
                
                if (pid_pos != std::string::npos && cpu_pos != std::string::npos) {
                    size_t pid_end = response.find(" ", pid_pos);
                    if (pid_end == std::string::npos) pid_end = response.find("\n", pid_pos);
                    
                    size_t cpu_end = response.find("%", cpu_pos);
                    if (cpu_end == std::string::npos) cpu_end = response.find("\n", cpu_pos);
                    
                    std::string pid_str = response.substr(pid_pos + 4, pid_end - (pid_pos + 4));
                    std::string cpu_str = response.substr(cpu_pos + 4, cpu_end - (cpu_pos + 4));
                    
                    // Извлекаем имя процесса из запроса
                    std::string process_name = command;
                    if (process_name.find("STATUS ") == 0) {
                        process_name = process_name.substr(7);
                    }
                    
                    print_table_header();
                    print_table_row(process_name, pid_str, cpu_str);
                } else {
                    std::cout << response;
                }
            } else if (response == "NOT_RUNNING\n") {
                std::cout << "[MISS] Process not found in system" << std::endl;
            } else if (response.find("ERROR:") != std::string::npos) {
                std::cerr << response;
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