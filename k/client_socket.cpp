#include "SocketWrapper.h"

ClientSocket::ClientSocket() : Socket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {
    if (!is_valid()) throw NetworkException("Failed to create client socket");
}

void ClientSocket::connect(const std::string& host, int port) {
    if (!is_valid()) throw NetworkException("Socket is invalid");

    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &address.sin_addr) <= 0) {
        addrinfo hints{}, *result = nullptr;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo(host.c_str(), nullptr, &hints, &result) != 0)
            throw NetworkException("Invalid host");
        
        address.sin_addr = reinterpret_cast<sockaddr_in*>(result->ai_addr)->sin_addr;
        freeaddrinfo(result);
    }

    if (::connect(socket_, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR)
        throw NetworkException("Connection failed");
}