#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

constexpr int BUFFER_SIZE = 1024;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    const int port = std::stoi(argv[1]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Enable broadcast
    int broadcastEnable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable,
                   sizeof(broadcastEnable)) < 0) {
        std::cerr << "Error setting broadcast option" << std::endl;
        close(sockfd);
        return 1;
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    serverAddr.sin_port = htons(port);

    char buffer[BUFFER_SIZE];

    while (true) {
        std::cout << "Enter message: ";
        std::cin.getline(buffer, BUFFER_SIZE);

        if (sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Error sending message" << std::endl;
            break;
        }

        if (strcmp(buffer, "exit") == 0) {
            std::cout << "Exiting chat..." << std::endl;
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
    }

    close(sockfd);
    return 0;
}
