#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cout << "Connection failed\n";
        return 1;
    }

    std::cout << "Connected to server\n";

    std::string line;
    char buffer[1024];

    while (true) {
        std::getline(std::cin, line);

        if (line == "exit")
            break;

        write(sock, line.c_str(), line.size());

        int bytes = read(sock, buffer, sizeof(buffer) - 1);

        if (bytes <= 0)
            break;

        buffer[bytes] = '\0';

        std::cout << "Server: " << buffer << "\n";
    }

    close(sock);
    return 0;
}
