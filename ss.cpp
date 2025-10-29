#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>
using namespace std;

#define NAMING_SERVER_IP "127.0.0.1"   // IP of the Naming Server
#define NAMING_SERVER_PORT 8080        // Port where Naming Server listens

int main() {
    int nm_sock = 0, ss_sock = 0;
    struct sockaddr_in nm_addr, ss_addr;
    char buffer[1024] = {0};

    // Create socket for Naming Server connection
    if ((nm_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    nm_addr.sin_family = AF_INET;
    nm_addr.sin_port = htons(NAMING_SERVER_PORT);

    if (inet_pton(AF_INET, NAMING_SERVER_IP, &nm_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return -1;
    }

    // Create a socket for this Storage Server (for clients)
    if ((ss_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Storage socket creation failed");
        return -1;
    }

    ss_addr.sin_family = AF_INET;
    ss_addr.sin_addr.s_addr = INADDR_ANY;
    ss_addr.sin_port = 0;  // Ask OS to assign a random available port

    if (bind(ss_sock, (struct sockaddr *)&ss_addr, sizeof(ss_addr)) < 0) {
        perror("Bind failed");
        return -1;
    }

    // Get assigned random port
    socklen_t addrlen = sizeof(ss_addr);
    getsockname(ss_sock, (struct sockaddr *)&ss_addr, &addrlen);
    int assignedPort = ntohs(ss_addr.sin_port);

    cout << "\n==============================\n";
    cout << "[+] Storage Server started" << endl;
    cout << "[+] Assigned random port: " << assignedPort << endl;

    // Start listening on the assigned port
    if (listen(ss_sock, 3) < 0) {
        perror("Listen failed");
        return -1;
    }

    // Connect to Naming Server
    if (connect(nm_sock, (struct sockaddr *)&nm_addr, sizeof(nm_addr)) < 0) {
        perror("Connection to Naming Server failed");
        return -1;
    }

    cout << "[+] Connected to Naming Server (" 
         << NAMING_SERVER_IP << ":" << NAMING_SERVER_PORT << ")" << endl;

    // Prepare registration message (only port info)
    string reg_msg = "REGISTER|PORT:" + to_string(assignedPort);
    send(nm_sock, reg_msg.c_str(), reg_msg.size(), 0);
    cout << "[→] Sent to Naming Server: " << reg_msg << endl;

    // Wait for acknowledgment
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = read(nm_sock, buffer, sizeof(buffer));
    if (bytesRead > 0)
        cout << "[←] Response from Naming Server: " << buffer << endl;
    else
        cout << "[!] No response or connection lost." << endl;

    cout << "[*] Storage Server is now active and waiting..." << endl;
    cout << "==============================\n";

    // Keep alive forever (simulate server running)
    while (true) {
        sleep(10);
    }

    close(ss_sock);
    close(nm_sock);
    return 0;
}
