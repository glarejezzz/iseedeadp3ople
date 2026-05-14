#include "SocketWrapper.h"

int main() {
    const int PORT = 12345;
    Server server;
    server.run(PORT);
    return 0;
}