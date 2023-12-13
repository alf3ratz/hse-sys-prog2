#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>

constexpr int MAX_BUFFER_SIZE = 1024;
constexpr int MAX_CLIENTS = 10;

void executeCommand(int clientSocket, const std::string& command) {
    std::stringstream response;

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        response << "Error: Unable to execute command.";
    } else {
        char buffer[MAX_BUFFER_SIZE];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            response << buffer;
        }
        pclose(pipe);
    }

    send(clientSocket, response.str().c_str(), response.str().length(), 0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int serverSocket, clientSocket;
    socklen_t clientLength;
    struct sockaddr_in serverAddress, clientAddress;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error: Unable to open socket." << std::endl;
        return 1;
    }

    // Initialize server address
    memset((char*)&serverAddress, 0, sizeof(serverAddress));
    int portNumber = std::atoi(argv[1]);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portNumber);

    // Bind socket to address
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error: Unable to bind socket." << std::endl;
        return 1;
    }

    // Listen for incoming connections
    listen(serverSocket, MAX_CLIENTS);
    std::cout << "Server listening on port " << portNumber << "..." << std::endl;

    clientLength = sizeof(clientAddress);

    while (true) {
        // Accept a connection
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLength);
        if (clientSocket < 0) {
            std::cerr << "Error: Unable to accept connection." << std::endl;
            continue;
        }

        // Fork a child process to handle the client
        if (fork() == 0) {
            close(serverSocket);  // Child process doesn't need the listener socket

            char buffer[MAX_BUFFER_SIZE];
            memset(buffer, 0, sizeof(buffer));

            // Receive command from the client
            ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead < 0) {
                std::cerr << "Error: Unable to read from socket." << std::endl;
            } else if (bytesRead == 0) {
                std::cout << "Connection closed by client." << std::endl;
            } else {
                // Execute the received command
                std::string command(buffer);
                executeCommand(clientSocket, command);
            }

            // Close the client socket and exit the child process
            close(clientSocket);
            exit(0);
        } else {
            // Parent process closes the client socket
            close(clientSocket);
        }
    }

    close(serverSocket);
    return 0;
}
