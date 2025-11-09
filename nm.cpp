#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <thread>
#include <map>
#include <mutex>

using namespace std;

#define PORT 8080

// Registry: IP -> Port
map<string, int> storageServers;
mutex storageMutex;

// ---------------------------------------------------------------------------
// Function to continuously monitor a storage server connection
// ---------------------------------------------------------------------------
void monitorStorageServer(int socket_fd, string clientIP, int registeredPort)
{
    char buffer[1024] = {0};
    cout << "[*] Monitoring Storage Server " << clientIP << ":" << registeredPort << endl;

    while (true)
    {
        ssize_t bytesRead = read(socket_fd, buffer, sizeof(buffer));
        if (bytesRead <= 0)
        {
            cout << "[x] Storage Server disconnected: " << clientIP << ":" << registeredPort << endl;

            // Remove from registry safely
            {
                lock_guard<mutex> lock(storageMutex);
                auto it = storageServers.find(clientIP);
                if (it != storageServers.end() && it->second == registeredPort)
                {
                    storageServers.erase(it);
                    cout << "[#] Removed from registry. Active servers: "
                         << storageServers.size() << endl;
                }
            }

            close(socket_fd);
            return;
        }

        string msg(buffer);
        cout << "[Storage " << clientIP << "] Message: " << msg << endl;

        // Optional: respond or process storage messages
        string ack = "ACK from Naming Server";
        send(socket_fd, ack.c_str(), ack.size(), 0);
    }
}

// ---------------------------------------------------------------------------
// Function to handle client communication
// ---------------------------------------------------------------------------
void handleClient(int socket_fd, string clientIP, int clientPort)
{
    char buffer[1024] = {0};
    cout << "[*] Started session with Client: " << clientIP << ":" << clientPort << endl;

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = read(socket_fd, buffer, sizeof(buffer));
        if (bytesRead <= 0)
        {
            cout << "[x] Client disconnected unexpectedly: " << clientIP << ":" << clientPort << endl;
            close(socket_fd);
            return;
        }

        string msg(buffer);
        cout << "[Client " << clientIP << "] Message: " << msg << endl;

        if (msg == "EXIT")
        {
            cout << "[x] Client requested disconnection: " << clientIP << ":" << clientPort << endl;

            close(socket_fd);
            return;
        }

        // Respond to normal messages
        const char *response = "Hello from Naming Server";
        send(socket_fd, response, strlen(response), 0);
    }
}

// ---------------------------------------------------------------------------
// Function to handle the initial handshake
// ---------------------------------------------------------------------------
void handleConnection(int socket_fd, sockaddr_in client_addr)
{
    char buffer[1024] = {0};
    char clientIP_cstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP_cstr, INET_ADDRSTRLEN);
    string clientIP(clientIP_cstr);

    cout << "\n[+] Connection established from: " << clientIP
         << " | Port: " << ntohs(client_addr.sin_port) << endl;

    // --- First message (handshake) ---
    ssize_t bytesRead = read(socket_fd, buffer, sizeof(buffer));
    if (bytesRead <= 0)
    {
        cout << "[!] No handshake data received from " << clientIP << endl;
        close(socket_fd);
        return;
    }

    string msg(buffer);
    cout << "[Handshake]: " << msg << endl;

    // --- Identify connection type ---
    if (msg.rfind("Storage Server", 0) == 0)
    {
        // Expect a registration message next: REGISTER|PORT:<port>
        memset(buffer, 0, sizeof(buffer));
        bytesRead = read(socket_fd, buffer, sizeof(buffer));
        if (bytesRead <= 0)
        {
            cout << "[!] No registration info from Storage Server " << clientIP << endl;
            close(socket_fd);
            return;
        }

        string regMsg(buffer);
        size_t pos = regMsg.find("PORT:");
        if (pos != string::npos)
        {
            int port = stoi(regMsg.substr(pos + 5));

            // Store in registry
            {
                lock_guard<mutex> lock(storageMutex);
                storageServers[clientIP] = port;
            }

            cout << "[+] Registered Storage Server: " << clientIP << ":" << port << endl;
            cout << "[#] Total registered storage servers: " << storageServers.size() << endl;

            string ack = "Storage Registration Successful";
            send(socket_fd, ack.c_str(), ack.size(), 0);

            // Launch monitor thread
            thread monitorThread(monitorStorageServer, socket_fd, clientIP, port);
            monitorThread.detach();
            return;
        }
        else
        {
            string err = "ERROR: Invalid registration format (expected REGISTER|PORT:<port>)";
            send(socket_fd, err.c_str(), err.size(), 0);
            close(socket_fd);
            return;
        }
    }
    else if (msg.rfind("Client", 0) == 0)
    {
        string ack = "Client connected successfully!";
        send(socket_fd, ack.c_str(), ack.size(), 0);

        int clientPort = ntohs(client_addr.sin_port);
        thread clientThread(handleClient, socket_fd, clientIP, clientPort);
        clientThread.detach();
        return;
    }
    else
    {
        string err = "ERROR: Unknown connection type. Send 'Client' or 'Storage Server' first.";
        send(socket_fd, err.c_str(), err.size(), 0);
        close(socket_fd);
        return;
    }
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main()
{
    int server_fd;
    struct sockaddr_in address, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket Creation Failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind to IP/Port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
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
    if (listen(server_fd, 10) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "[*] Waiting for incoming connections...\n";

    // Infinite loop — keeps accepting clients and storage servers
    while (true)
    {
        int new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (new_socket < 0)
        {
            perror("Accept failed");
            continue;
        }

        thread t(handleConnection, new_socket, client_addr);
        t.detach();
    }

    close(server_fd);
    return 0;
}
