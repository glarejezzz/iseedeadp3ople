#include "SocketWrapper.h"

Socket::Socket(SOCKET socket) : socket_(socket) {};

Socket::Socket(Socket&& other) noexcept : socket_(std::move(other.socket_)) { 
    other.socket_ = INVALID_SOCKET; 
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this == &other) return *this;
    close();
    socket_ = std::move(other.socket_);
    other.socket_ = INVALID_SOCKET;
    return *this;
}

Socket::~Socket() { 
    close(); 
}

void Socket::close() {
    if (socket_ != INVALID_SOCKET) {
        closesocket(socket_);
        socket_ = INVALID_SOCKET;
    }
}

bool Socket::is_valid() const { 
    return socket_ != INVALID_SOCKET; 
}

SOCKET Socket::get_handle() const { 
    return socket_; 
}

void Socket::send(const std::string& str) {
    send(str.c_str(), str.length());
}

void Socket::send(const void* data, size_t sz) {
    if (!is_valid()) throw NetworkException("Socket is not valid");
    const char* p = static_cast<const char*>(data);
    size_t total = 0;
    while (total < sz) {
        int sent = ::send(socket_, p + total, static_cast<int>(sz - total), 0);
        if (sent == SOCKET_ERROR) throw NetworkException("Send failed");
        total += sent;
    }
}

size_t Socket::receive(void* buffer, size_t sz) {
    if (!is_valid()) throw NetworkException("Socket is not valid");
    int received = ::recv(socket_, static_cast<char*>(buffer), static_cast<int>(sz), 0);
    if (received == SOCKET_ERROR) {
        if (WSAGetLastError() == WSAECONNRESET) return 0;
        throw NetworkException("Receive failed");
    }
    return received;
}

std::string Socket::receive(size_t max_sz) {
    std::string result(max_sz, '\0');
    size_t received = receive(&result[0], max_sz);
    result.resize(received);
    return result;
}

std::string Socket::receive_line() {
    std::string line;
    char ch;
    while (true) {
        size_t received = receive(&ch, 1);
        if (received == 0) break;
        if (ch == '\n') break;
        if (ch != '\r') line += ch;
    }
    return line;
}