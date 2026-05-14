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
#include <algorithm>
#include <string>
#include <cstring>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class ChatHub {
private:
    struct ClientInfo {
        std::unique_ptr<Socket> socket;
        std::string nickname;
        std::string ip;
        int port;
        std::chrono::steady_clock::time_point last_ping;
        bool active;
    };

    std::vector<std::unique_ptr<ClientInfo>> clients_;
    std::shared_mutex clients_mutex_;
    std::ofstream log_file_;
    std::mutex log_mutex_;
    std::atomic<bool> running_{true};
    std::thread ping_thread_;

    std::string get_current_time() {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::tm bt;
        localtime_s(&bt, &now_time_t);
        
        std::ostringstream oss;
        oss << std::put_time(&bt, "%Y-%m-%d %H:%M:%S") 
            << "." << std::setfill('0') << std::setw(3) << now_ms.count();
        return oss.str();
    }

    void write_log(const std::string& ip_port, const std::string& nickname, 
                   const std::string& message) {
        std::lock_guard<std::mutex> lock(log_mutex_);
        log_file_ << "[" << get_current_time() << "] [" << ip_port << "] " 
                  << nickname << ": " << message << std::endl;
        log_file_.flush();
    }

    void broadcast_message(const std::string& message, const std::string& sender_nickname, 
                          const std::string& sender_ip_port, 
                          ClientInfo* sender_info = nullptr) {
        std::shared_lock<std::shared_mutex> lock(clients_mutex_);
        
        for (auto& client : clients_) {
            if (!client->active) continue;
            
            if (sender_info && client.get() == sender_info) {
                continue;
            }
            
            try {
                client->socket->send(message);
            } catch (const NetworkException& e) {
                std::cout << "[Server] Failed to send to " << client->nickname << std::endl;
            }
        }
        
        write_log(sender_ip_port, sender_nickname, message.substr(message.find(']') + 2));
    }

    void remove_client(size_t index) {
        if (index >= clients_.size()) return;
        
        auto& client = clients_[index];
        if (!client->active) return;
        
        std::string nickname = client->nickname;
        std::string ip_port = client->ip + ":" + std::to_string(client->port);
        
        client->active = false;
        
        try {
            client->socket->close();
        } catch (...) {}
        
        std::string disconnect_msg = "[SYSTEM] " + nickname + " disconnected\n";
        
        {
            std::shared_lock<std::shared_mutex> lock(clients_mutex_);
            for (auto& other : clients_) {
                if (other->active && other.get() != client.get()) {
                    try {
                        other->socket->send(disconnect_msg);
                    } catch (const NetworkException& e) {}
                }
            }
        }
        
        write_log(ip_port, "SYSTEM", "disconnected");
        std::cout << "[Server] " << nickname << " disconnected" << std::endl;
        
        clients_.erase(clients_.begin() + index);
    }

public:
    ChatHub() {
        log_file_.open("chat.log", std::ios::app);
        if (!log_file_.is_open()) {
            throw NetworkException("Failed to open log file");
        }
        write_log("SYSTEM", "CHAT", "Chat server started");
        std::cout << "[ChatHub] Chat server initialized, logging to chat.log" << std::endl;
        
        ping_thread_ = std::thread(&ChatHub::ping_clients, this);
    }

    ~ChatHub() {
        running_ = false;
        if (ping_thread_.joinable()) {
            ping_thread_.join();
        }
        
        {
            std::unique_lock<std::shared_mutex> lock(clients_mutex_);
            for (auto& client : clients_) {
                try {
                    client->socket->close();
                } catch (...) {}
            }
            clients_.clear();
        }
        
        write_log("SYSTEM", "CHAT", "Chat server stopped");
        log_file_.close();
    }

    void ping_clients() {
        while (running_) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            
            if (!running_) break;
            
            std::unique_lock<std::shared_mutex> lock(clients_mutex_);
            auto now = std::chrono::steady_clock::now();
            
            for (size_t i = 0; i < clients_.size(); ) {
                auto& client = clients_[i];
                if (!client->active) {
                    ++i;
                    continue;
                }
                
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - client->last_ping).count();
                
                if (elapsed > 10) {
                    std::cout << "[Server] " << client->nickname << " timed out" << std::endl;
                    lock.unlock();
                    remove_client(i);
                    lock.lock();
                } else if (elapsed > 5) {
                    try {
                        client->socket->send("PING\n");
                        client->last_ping = now;
                        ++i;
                    } catch (const NetworkException& e) {
                        lock.unlock();
                        remove_client(i);
                        lock.lock();
                    }
                } else {
                    ++i;
                }
            }
        }
    }

    void handle_client(std::unique_ptr<Socket> client_socket, const std::string& ip, int port) {
        std::string ip_port = ip + ":" + std::to_string(port);
        
        std::string nickname;
        try {
            std::string initial_msg = client_socket->receive(256);
            if (initial_msg.empty()) return;
            
            while (!initial_msg.empty() && (initial_msg.back() == '\n' || initial_msg.back() == '\r')) {
                initial_msg.pop_back();
            }
            
            if (initial_msg.find("NICK ") != 0) {
                client_socket->send("ERROR: invalid_protocol\n");
                return;
            }
            
            nickname = initial_msg.substr(5);
            if (nickname.empty()) {
                client_socket->send("ERROR: empty_nickname\n");
                return;
            }
            
            {
                std::shared_lock<std::shared_mutex> lock(clients_mutex_);
                for (auto& client : clients_) {
                    if (client->active && client->nickname == nickname) {
                        client_socket->send("ERROR: nickname_taken\n");
                        return;
                    }
                }
            }
            
        } catch (const NetworkException& e) {
            std::cout << "[Server] Failed to receive nickname: " << e.what() << std::endl;
            return;
        }
        
        auto client_info = std::make_unique<ClientInfo>();
        client_info->socket = std::move(client_socket);
        client_info->nickname = nickname;
        client_info->ip = ip;
        client_info->port = port;
        client_info->last_ping = std::chrono::steady_clock::now();
        client_info->active = true;
        
        try {
            client_info->socket->send("WELCOME\n");
        } catch (const NetworkException& e) {
            return;
        }
        
        {
            std::unique_lock<std::shared_mutex> lock(clients_mutex_);
            clients_.push_back(std::move(client_info));
        }
        
        std::string join_msg = "[SYSTEM] " + nickname + " connected\n";
        
        {
            std::shared_lock<std::shared_mutex> lock(clients_mutex_);
            for (auto& client : clients_) {
                if (client->active && client->nickname != nickname) {
                    try {
                        client->socket->send(join_msg);
                    } catch (const NetworkException& e) {}
                }
            }
        }
        
        write_log(ip_port, nickname, "connected");
        std::cout << "[Server] " << nickname << " connected from " << ip_port << std::endl;
        
        // Найти добавленного клиента и обрабатывать его сообщения
        ClientInfo* client_ptr = nullptr;
        {
            std::shared_lock<std::shared_mutex> lock(clients_mutex_);
            for (auto& client : clients_) {
                if (client->nickname == nickname && client->active) {
                    client_ptr = client.get();
                    break;
                }
            }
        }
        
        if (!client_ptr) return;
        
        try {
            while (running_ && client_ptr->active) {
                std::string message = client_ptr->socket->receive(4096);
                
                if (message.empty()) break;
                
                while (!message.empty() && (message.back() == '\n' || message.back() == '\r')) {
                    message.pop_back();
                }
                
                if (message.empty()) continue;
                
                if (message == "PONG") {
                    client_ptr->last_ping = std::chrono::steady_clock::now();
                    continue;
                }
                
                std::string broadcast_msg = "[" + nickname + "] " + message + "\n";
                client_ptr->last_ping = std::chrono::steady_clock::now();
                
                {
                    std::shared_lock<std::shared_mutex> lock(clients_mutex_);
                    for (auto& client : clients_) {
                        if (client->active && client.get() != client_ptr) {
                            try {
                                client->socket->send(broadcast_msg);
                            } catch (const NetworkException& e) {}
                        }
                    }
                }
                
                write_log(ip_port, nickname, message);
            }
        } catch (const NetworkException& e) {
            std::cout << "[Server] Client " << nickname << " error: " << e.what() << std::endl;
        }
        
        {
            std::unique_lock<std::shared_mutex> lock(clients_mutex_);
            for (size_t i = 0; i < clients_.size(); ++i) {
                if (clients_[i]->nickname == nickname && clients_[i]->active) {
                    lock.unlock();
                    remove_client(i);
                    break;
                }
            }
        }
    }
};

void Server::run(int port) {
    try {
        WSAInitializer wsa;
        ServerSocket server(port, false);
        std::cout << "[Server] Broadcast Chat server listening on port " << port << std::endl;
        std::cout << "[Server] Clients should send: NICK <nickname>" << std::endl;

        ChatHub hub;
        std::vector<std::thread> client_threads;

        while (true) {
            auto client_socket = server.accept();
            
            sockaddr_in addr;
            int addr_len = sizeof(addr);
            std::string client_ip = "unknown";
            int client_port = 0;
            
            if (getpeername(client_socket->get_handle(), (sockaddr*)&addr, &addr_len) == 0) {
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
                client_ip = ip;
                client_port = ntohs(addr.sin_port);
            }
            
            std::thread client_thread([&hub, client_socket = std::move(client_socket), 
                                       client_ip, client_port]() mutable {
                hub.handle_client(std::move(client_socket), client_ip, client_port);
            });
            client_thread.detach();
        }
    } catch (const NetworkException& e) {
        std::cerr << "[Server] Fatal error: " << e.what() << std::endl;
    }
}