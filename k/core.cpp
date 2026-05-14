#include "SocketWrapper.h"

NetworkException::NetworkException(const std::string &message)
    : std::runtime_error(message + " (Error: " + std::to_string(WSAGetLastError()) + ")") {}

WSAInitializer::WSAInitializer() {
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        throw NetworkException("WSAStartup failed");
}

WSAInitializer::~WSAInitializer() {
    WSACleanup();
}
