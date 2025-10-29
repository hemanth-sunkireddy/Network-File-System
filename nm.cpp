#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <thread>
#include <map>

using namespace std;

#define PORT 8080

// Registry: IP -> Port (not thread-safe now)
map<string, int> storageServers;

// Function to continuously monitor a storage server connection
void monitorStorageServer(int socket_fd, string clientIP, int registeredPort) {
    char buffer[1024] = {0};
    cout << "[*] Monitoring Storage Server " << clientIP << ":" << registeredPort << endl;

    // Continuously read from socket (blocking until closed)
    while (true) {
        ssize_t bytesRead = read(socket_fd, buffer, sizeof(buffer));
        if (bytesRead <= 0) {
            // Storage server disconnected
            cout << "[x] Storage Server disconnected: " << clientIP << ":" << registeredPort << endl;

            // Remove from registry
            auto it = storageServers.find(clientIP);
            if (it != storageServers.end() && it->second == registeredPort) {
                storageServers.erase(it);
                cout << "[#] Removed from registry. Active servers: "
                     << storageServers.size() << endl;
            }

            close(socket_fd);
            return;
        }
    }
}

// Function to handle each connection
void handleConnection(int socket_fd, sockaddr_in client_addr) {
    char buffer[1024] = {0};
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP, INET_ADDRSTRLEN);

    cout << "\n[+] Connection established from: " << clientIP
         << " | Port: " << ntohs(client_addr.sin_port) << endl;

    // Read message from connection
    ssize_t bytesRead = read(socket_fd, buffer, sizeof(buffer));
    if (bytesRead <= 0) {
        cout << "[!] No data received from " << clientIP << endl;
        close(socket_fd);
        return;
    }

    string msg(buffer);
    cout << "[Received]: " << msg << endl;

    // Detect Storage Server registration message
    if (msg.rfind("REGISTER", 0) == 0) {
        size_t pos = msg.find("PORT:");
        if (pos != string::npos) {
            int port = stoi(msg.substr(pos + 5));

            // Store info (not thread-safe)
            storageServers[clientIP] = port;

            cout << "[+] Registered Storage Server: " << clientIP << ":" << port << endl;
            cout << "[#] Total registered storage servers: " << storageServers.size() << endl;

            string ack = "Registration Successful";
            send(socket_fd, ack.c_str(), ack.size(), 0);

            // Start a monitor thread to detect disconnects
            thread monitorThread(monitorStorageServer, socket_fd, clientIP, port);
            monitorThread.detach();
            return; // Don’t close socket; thread will manage it
        } else {
            string err = "ERROR: Invalid registration format";
            send(socket_fd, err.c_str(), err.size(), 0);
            close(socket_fd);
        }
    } else {
        // Normal client message (e.g., hello or request)
        cout << "[Client " << clientIP << "] Message: " << msg << endl;
        const char* response = "Hello from Naming Server";
        send(socket_fd, response, strlen(response), 0);
        close(socket_fd);
        cout << "[x] Client connection closed for: " << clientIP << endl;
    }
}

// Main function
int main() {
    int server_fd;
    struct sockaddr_in address, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket Creation Failed");
        exit(EXIT_FAILURE);
    }

    // Bind to IP/Port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }

    // Print startup info
    char hostIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), hostIP, INET_ADDRSTRLEN);
    cout << "\n==============================\n";
    cout << "Naming Server started successfully!" << endl;
    cout << "Listening on IP: " << hostIP << " | Port: " << ntohs(address.sin_port) << endl;
    cout << "==============================\n";

    // Listen for connections
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "[*] Waiting for incoming connections...\n";

    // Infinite loop — keeps accepting clients and storage servers
    while (true) {
        int new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Handle each connection in a new thread
        thread t(handleConnection, new_socket, client_addr);
        t.detach();
    }

    close(server_fd);
    return 0;
}
