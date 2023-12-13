// #include <arpa/inet.h>
// #include <unistd.h>

// #include <cstdio>
// #include <cstdlib>
// #include <cstring>
// #include <iostream>
// #include <thread>

// const int BUFFER_SIZE = 1024;

// void receiveMessages(int socket) {
//     char buffer[BUFFER_SIZE];
//     sockaddr_in senderAddress;
//     socklen_t senderLength;

//     while (true) {
//         memset(buffer, 0, sizeof(buffer));
//         senderLength = sizeof(senderAddress);

//         ssize_t bytesRead =
//             recvfrom(socket, buffer, sizeof(buffer), 0,
//                      (struct sockaddr*)&senderAddress, &senderLength);
//         if (bytesRead <= 0) {
//             std::cerr << "Nothing recieved\n" << std::endl;
//             break;
//         }

//         std::cout << "Received from " << inet_ntoa(senderAddress.sin_addr)
//                   << ": " << buffer << std::endl;
//     }
// }

// void sendMessages(int socket, const sockaddr_in& serverAddress) {
//     char buffer[BUFFER_SIZE];

//     while (true) {
//         memset(buffer, 0, sizeof(buffer));
//         std::cout << "Enter message: ";
//         std::cin.getline(buffer, sizeof(buffer));

//         if (sendto(socket, buffer, strlen(buffer), 0,
//                    (const struct sockaddr*)&serverAddress,
//                    sizeof(serverAddress)) <= 0) {
//             std::cerr << "Error sending message." << std::endl;
//             break;
//         }
//     }
// }

// int main(int argc, char* argv[]) {
//     if (argc != 2) {
//         std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
//         return -1;
//     }

//     int clientSocket;
//     sockaddr_in serverAddress;
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_port = htons(std::atoi(argv[1]));
//     serverAddress.sin_addr.s_addr = INADDR_ANY;

//     if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         std::cerr << "Error creating socket" << std::endl;
//         return -1;ads
//     }

//     if (connect(clientSocket, (struct sockaddr*)&serverAddress,
//                 sizeof(serverAddress)) < 0) {
//         std::cerr << "Error connecting to server. Error code: " << errno
//                   << std::endl;
//         //close(clientSocket);
//         if (bind(clientSocket, (struct sockaddr*)&serverAddress,
//                  sizeof(serverAddress)) < 0) {
//             std::cerr << "Error binding socket." << errno << std::endl;
//             close(clientSocket);
//             return -1;
//         }

//         std::cout << "First client is waiting for connections..." <<
//         std::endl;

//         if (listen(clientSocket, 10) < 0) {
//             std::cerr << "Error listening for connections." << std::endl;
//             close(clientSocket);
//             return -1;
//         }
//     } else {
//         std::cout << "Connected to chat!" << std::endl;
//     }

//     std::thread receiveThread(receiveMessages, clientSocket);
//     std::thread sendThread(sendMessages, clientSocket, serverAddress);

//     receiveThread.join();
//     sendThread.join();

//     close(clientSocket);

//     return 0;
// }
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

const int BUFFER_SIZE = 1024;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return -1;
    }

    int clientSocket;
    sockaddr_in serverAddress, clientAddress;

    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(std::atoi(argv[1]));
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(clientSocket, (struct sockaddr*)&serverAddress,
             sizeof(serverAddress)) < 0) {
        std::cerr << "Error binding socket." << std::endl;
        std::cout << "Trying to connect as a client ... " << '\n';
        if (connect(clientSocket, (struct sockaddr*)&serverAddress,
                    sizeof(serverAddress)) < 0) {
            std::cerr << "Error connecting to server. Error code: " << errno
                      << std::endl;
            close(clientSocket);
            return -1;
        } else {
            std::cout << "Connected as a client!" << std::endl;
        }
        // close(clientSocket);
        // return -1;
    } else {
        std::cout << "Connected as a server!" << std::endl;
    }

    char buffer[BUFFER_SIZE];

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        // sockaddr_in senderAddress;
        socklen_t senderLength = sizeof(serverAddress);
        // std::cout << "1." << std::endl;
        std::cout << "Enter message: ";
        std::cin.getline(buffer, sizeof(buffer));
        sendto(clientSocket, buffer, strlen(buffer), 0,
               (const struct sockaddr*)&serverAddress, senderLength);
        // std::cout << "2." << std::endl;
        ssize_t bytesRead =
            recvfrom(clientSocket, buffer, sizeof(buffer), 0,
                     (struct sockaddr*)&serverAddress, &senderLength);
        // std::cout << "3." << std::endl;
        if (bytesRead <= 0) {
            std::cerr << "Error receiving message." << std::endl;
            break;
        }

        std::cout << "Received from " << inet_ntoa(serverAddress.sin_addr)
                  << ": " << buffer << std::endl;

        // Respond to the sender
    }

    close(clientSocket);

    return 0;
}
