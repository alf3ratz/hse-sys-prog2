#include <arpa/inet.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <set>
#include <vector>


const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

std::set<int> clientSockets;

void broadcastMessage(int senderSocket, const std::string& message) {
    for (int clientSocket : clientSockets) {
        if (clientSocket != senderSocket) {
            send(clientSocket, message.c_str(), message.length(), 0);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    const int port = std::stoi(argv[1]);
    int serverSocket;
    sockaddr_in serverAddress;

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress,
             sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket." << std::endl;
        close(serverSocket);
        return -1;
    }

    std::cout << "Server is waiting for connections..." << std::endl;

    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        std::cerr << "Error listening for connections." << std::endl;
        close(serverSocket);
        return -1;
    }

    fd_set readfds;
    int maxSocket = serverSocket;

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(serverSocket, &readfds);

        for (int clientSocket : clientSockets) {
            FD_SET(clientSocket, &readfds);
            maxSocket = std::max(maxSocket, clientSocket);
        }

        if (select(maxSocket + 1, &readfds, nullptr, nullptr, nullptr) == -1) {
            std::cerr << "Error in select." << std::endl;
            break;
        }

        // Handle new connection
        if (FD_ISSET(serverSocket, &readfds)) {
            int newClientSocket;
            sockaddr_in clientAddress;
            socklen_t clientLength = sizeof(clientAddress);

            if ((newClientSocket =
                     accept(serverSocket, (struct sockaddr*)&clientAddress,
                            &clientLength)) == -1) {
                std::cerr << "Error accepting connection." << std::endl;
                continue;
            }

            std::cout << "Connection accepted from "
                      << inet_ntoa(clientAddress.sin_addr) << std::endl;

            clientSockets.insert(newClientSocket);
        }

        // Handle data from clients
        for (auto it = clientSockets.begin(); it != clientSockets.end();
             /* no increment here */) {
            int clientSocket = *it;

            if (FD_ISSET(clientSocket, &readfds)) {
                char buffer[BUFFER_SIZE];
                memset(buffer, 0, sizeof(buffer));

                ssize_t bytesRead =
                    recv(clientSocket, buffer, sizeof(buffer), 0);

                if (bytesRead <= 0) {
                    std::cout << "Client disconnected." << std::endl;
                    close(clientSocket);
                    it = clientSockets.erase(it);
                    continue;
                }

                std::cout << "Received from " << clientSocket << ": " << buffer
                          << std::endl;

                // Broadcast the message to other clients
                broadcastMessage(clientSocket, buffer);
            }

            ++it;
        }
    }

    // Close all client sockets
    for (int clientSocket : clientSockets) {
        close(clientSocket);
    }

    close(serverSocket);

    return 0;
}
