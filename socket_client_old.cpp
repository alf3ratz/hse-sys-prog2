#include <arpa/inet.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <sys/socket.h>

const int BUFFER_SIZE = 1024;

void receiveMessages(int clientSocket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        if (recv(clientSocket, buffer, sizeof(buffer), 0) <= 0) {
            std::cerr << "Error receiving message." << std::endl;
            break;
        }
        std::cout << "Received: " << buffer << std::endl;
    }
}

void sendMessages(int clientSocket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        std::cout << "Enter message: ";
        std::cin.getline(buffer, sizeof(buffer));

        if (send(clientSocket, buffer, strlen(buffer), 0) <= 0) {
            std::cerr << "Error sending message." << std::endl;
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    const int port = std::stoi(argv[1]);
    int clientSocket;
    sockaddr_in serverAddress;

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    // if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
    //     std::cerr << "Invalid address error" << std::endl;
    //     return -1;
    // } else {
    //     std::cout << "ok1" << std::endl;
    // }

    // connect(clientSocket, (struct sockaddr*)&serverAddress,
    //             sizeof(serverAddress));
    if (connect(clientSocket, (struct sockaddr *)&serverAddress,
                sizeof(serverAddress)) < 0) {
        std::cerr << "Error connecting to server. Error code: " << errno << std::endl;
        close(clientSocket);
        return -1;
    } else {
        std::cout << "ok2" << std::endl;
    }

    std::cout << "Connected to the server." << std::endl;

    std::thread receiveThread(receiveMessages, clientSocket);
    std::thread sendThread(sendMessages, clientSocket);

    receiveThread.join();
    sendThread.join();

    close(clientSocket);

    return 0;
}
