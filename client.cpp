#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>
using namespace std;

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr, local_addr;
    char buffer[1024] = {0};
    string message;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        return -1;
    }

    // Connect to the Naming Server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    // Get local client details (IP & Port)
    socklen_t addrlen = sizeof(local_addr);
    getsockname(sock, (struct sockaddr *)&local_addr, &addrlen);

    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(local_addr.sin_addr), clientIP, INET_ADDRSTRLEN);
    int clientPort = ntohs(local_addr.sin_port);

    // Print connection info
    cout << "\n==============================" << endl;
    cout << "✅ Connected to Naming Server" << endl;
    cout << "------------------------------" << endl;
    cout << "Client IP   : " << clientIP << endl;
    cout << "Client Port : " << clientPort << endl;
    cout << "Server IP   : " << SERVER_IP << endl;
    cout << "Server Port : " << PORT << endl;
    cout << "==============================\n" << endl;

    // Main loop for user options
    while (true) {
        cout << "\nChoose an option:" << endl;
        cout << "1. Send Hello" << endl;
        cout << "2. Exit" << endl;
        cout << "Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();  // Clear newline

        if (choice == 1) {
            message = "Hello from client";

            ssize_t bytesSent = send(sock, message.c_str(), message.length(), 0);
            if (bytesSent <= 0) {
                cout << "[!] Failed to send message. Connection might be lost." << endl;
                break;
            }

            cout << "\n→ Sent message to server: " << message << endl;

            memset(buffer, 0, sizeof(buffer));
            ssize_t bytesRead = read(sock, buffer, sizeof(buffer));
            if (bytesRead > 0) {
                cout << "← Received response from server: " << buffer << endl;
            } else {
                cout << "[!] No response or connection lost." << endl;
                break;
            }

        } else if (choice == 2) {
            cout << "\n[x] Closing connection and exiting..." << endl;
            break;

        } else {
            cout << "[!] Invalid choice. Try again." << endl;
        }
    }

    // Close socket
    close(sock);
    cout << "\n💤 Disconnected from Naming Server." << endl;
    cout << "Client (" << clientIP << ":" << clientPort << ") terminated gracefully.\n" << endl;

    return 0;
}
