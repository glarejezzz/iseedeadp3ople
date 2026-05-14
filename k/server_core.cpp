#include "SocketWrapper.h"
#include <iostream>
#include <map>
#include <shared_mutex>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <atomic>
#include <ctime>
#include <iomanip>
#include <vector>

class KeyValueStore {
private:
    std::map<std::string, std::string> data_;
    mutable std::shared_mutex mutex_;
    std::string dump_file_;
    std::atomic<bool> running_{true};
    std::thread dump_thread_;
    std::ofstream log_file_;
    std::mutex log_mutex_;

    void write_log(const std::string& command, const std::string& key, const std::string& client_ip) {
        auto now = std::time(nullptr);
        std::string timestamp = std::ctime(&now);
        timestamp.pop_back();
        
        std::lock_guard<std::mutex> lock(log_mutex_);
        log_file_ << "[" << timestamp << "] " << client_ip << " " << command << " " << key << std::endl;
        log_file_.flush();
    }

public:
    KeyValueStore(const std::string& dump_path = "store_dump.txt") 
        : dump_file_(dump_path) {
        log_file_.open("server_kv.log", std::ios::app);
        if (!log_file_.is_open()) {
            throw NetworkException("Failed to open log file");
        }
        load_from_dump();
        dump_thread_ = std::thread(&KeyValueStore::periodic_dump, this);
    }

    ~KeyValueStore() {
        running_ = false;
        if (dump_thread_.joinable()) {
            dump_thread_.join();
        }
        dump_to_file();
        log_file_.close();
    }

    void load_from_dump() {
        std::ifstream file(dump_file_);
        if (!file.is_open()) return;

        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find(':');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                std::lock_guard<std::shared_mutex> lock(mutex_);
                data_[key] = value;
            }
        }
    }

    void dump_to_file() {
        std::ofstream file(dump_file_);
        std::shared_lock<std::shared_mutex> lock(mutex_);
        for (const auto& pair : data_) {
            file << pair.first << ":" << pair.second << "\n";
        }
    }

    void periodic_dump() {
        while (running_) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            if (running_) {
                dump_to_file();
            }
        }
    }

    std::string process_command(const std::string& cmd, const std::string& client_ip) {
        std::istringstream iss(cmd);
        std::string command;
        iss >> command;

        if (command == "SET") {
            std::string key, value;
            iss >> key;
            std::getline(iss, value);
            if (!value.empty() && value[0] == ' ') value = value.substr(1);

            if (key.empty() || key.find(' ') != std::string::npos) {
                return "ERROR: invalid_key\n";
            }
            if (value.empty()) {
                return "ERROR: empty_value\n";
            }

            {
                std::lock_guard<std::shared_mutex> lock(mutex_);
                data_[key] = value;
            }
            write_log("SET", key, client_ip);
            return "OK\n";
        }
        else if (command == "GET") {
            std::string key;
            iss >> key;

            if (key.empty() || key.find(' ') != std::string::npos) {
                return "ERROR: invalid_key\n";
            }

            {
                std::shared_lock<std::shared_mutex> lock(mutex_);
                if (data_.empty()) {
                    return "ERROR: storage_empty\n";
                }
                auto it = data_.find(key);
                if (it != data_.end()) {
                    write_log("GET", key, client_ip);
                    return "VALUE: " + it->second + "\n";
                }
            }
            write_log("GET", key, client_ip);
            return "NOT_FOUND\n";
        }
        else if (command == "DEL") {
            std::string key;
            iss >> key;

            if (key.empty() || key.find(' ') != std::string::npos) {
                return "ERROR: invalid_key\n";
            }

            {
                std::lock_guard<std::shared_mutex> lock(mutex_);
                auto it = data_.find(key);
                if (it != data_.end()) {
                    data_.erase(it);
                    write_log("DEL", key, client_ip);
                    return "DELETED\n";
                }
            }
            write_log("DEL", key, client_ip);
            return "NOT_FOUND\n";
        }
        else if (command == "QUIT") {
            return "QUIT\n";
        }
        else {
            return "ERROR: unknown_command\n";
        }
    }
};

void handle_client(std::unique_ptr<Socket> client_socket, const std::string& client_ip, KeyValueStore& store) {
    std::cout << "[Server] Client connected from " << client_ip << std::endl;
    
    try {
        while (true) {
            std::string request = client_socket->receive(4096);
            if (request.empty()) break;
            
            // Удаляем символы новой строки
            while (!request.empty() && (request.back() == '\n' || request.back() == '\r')) {
                request.pop_back();
            }
            
            std::string response = store.process_command(request, client_ip);
            client_socket->send(response);
            
            if (request == "QUIT") {
                break;
            }
        }
    } catch (const NetworkException& e) {
        std::cout << "[Server] Client " << client_ip << " error: " << e.what() << std::endl;
    }
    
    std::cout << "[Server] Client " << client_ip << " disconnected" << std::endl;
}

void Server::run(int port) {
    try {
        WSAInitializer wsa;
        ServerSocket server(port, false);
        std::cout << "[Server] KV-Store server listening on port " << port << std::endl;

        KeyValueStore store;
        std::vector<std::thread> client_threads;

        while (true) {
            auto client_socket = server.accept();
            
            // Получаем IP клиента
            sockaddr_in addr;
            int addr_len = sizeof(addr);
            std::string client_ip = "unknown";
            if (getpeername(client_socket->get_handle(), (sockaddr*)&addr, &addr_len) == 0) {
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
                client_ip = ip;
            }
            
            // Запускаем клиента в отдельном потоке для поддержки нескольких подключений
            client_threads.emplace_back(handle_client, std::move(client_socket), client_ip, std::ref(store));
            client_threads.back().detach();
        }
    } catch (const NetworkException& e) {
        std::cerr << "[Server] Fatal error: " << e.what() << std::endl;
    }
}