#include "SocketWrapper.h"

ServerSocket::ServerSocket(int port, bool local_host_only) 
    : Socket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {
    if (!is_valid()) throw NetworkException("Failed to create server socket");

    int opt = 1;
    setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR
             , reinterpret_cast<char*>(&opt), sizeof(opt));
    
    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = local_host_only ? htonl(INADDR_LOOPBACK) : INADDR_ANY;

    if (::bind(socket_, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR)
        throw NetworkException("Bind failed");
    if (::listen(socket_, SOMAXCONN) == SOCKET_ERROR)
        throw NetworkException("Listen failed");
}

std::unique_ptr<Socket> ServerSocket::accept() {
    if (!is_valid()) throw NetworkException("Server socket is broken");
    SOCKET client_socket = ::accept(socket_, nullptr, nullptr);
    if (client_socket == INVALID_SOCKET)
        throw NetworkException("Accept failed");
    return std::make_unique<Socket>(client_socket);
}