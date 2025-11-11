#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <csignal>
#include <netdb.h>
#include <fstream>
#include <sstream>
using namespace std;

#define NAMING_SERVER_IP "127.0.0.1"
#define NAMING_SERVER_PORT 8080

int nm_sock = 0; // Naming Server socket
int ss_sock = 0; // Storage Server's listening socket
bool running = true;

// ---------------------------------------------------------------------------
// Graceful shutdown handler
// ---------------------------------------------------------------------------
void handleExit(int signum)
{
    cout << "\n[x] Caught signal (" << signum << "). Shutting down Storage Server gracefully..." << endl;

    if (nm_sock > 0)
    {
        string exitMsg = "EXIT";
        send(nm_sock, exitMsg.c_str(), exitMsg.size(), 0);
        close(nm_sock);
    }

    if (ss_sock > 0)
        close(ss_sock);

    running = false;
}

// ---------------------------------------------------------------------------
// Helper to get the local IP
// ---------------------------------------------------------------------------
string getLocalIP()
{
    char hostbuffer[256];
    char *IPbuffer;
    struct hostent *host_entry;

    if (gethostname(hostbuffer, sizeof(hostbuffer)) == -1)
    {
        perror("gethostname");
        return "127.0.0.1";
    }

    host_entry = gethostbyname(hostbuffer);
    if (host_entry == nullptr)
    {
        perror("gethostbyname");
        return "127.0.0.1";
    }

    IPbuffer = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));
    return string(IPbuffer);
}

// ---------------------------------------------------------------------------
// Function to handle client command
// Format: COMMAND|filename|data
// ---------------------------------------------------------------------------
string handleClientCommand(const string &msg)
{
    stringstream ss(msg);
    string command, filename, data;

    getline(ss, command, '|');
    getline(ss, filename, '|');
    getline(ss, data, '|'); // optional for WRITE

    if (command.empty() || filename.empty())
        return "[ERROR] Invalid command format.";

    if (command == "READ")
    {
        ifstream infile(filename);
        if (!infile)
            return "[ERROR] File not found: " + filename;

        stringstream buffer;
        buffer << infile.rdbuf();
        infile.close();
        return "[READ OK] " + buffer.str();
    }
    else if (command == "WRITE")
    {
        ofstream outfile(filename, ios::trunc);
        if (!outfile)
            return "[ERROR] Cannot open file for writing: " + filename;

        outfile << data;
        outfile.close();
        return "[WRITE OK] Data written to " + filename;
    }
    else if (command == "CREATE")
    {
        ofstream outfile(filename);
        if (!outfile)
            return "[ERROR] Cannot create file: " + filename;

        outfile.close();
        return "[CREATE OK] File created: " + filename;
    }
    else
    {
        return "[ERROR] Unknown command: " + command;
    }
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main()
{
    signal(SIGINT, handleExit);

    struct sockaddr_in nm_addr, ss_addr, clientAddr;
    char buffer[2048] = {0};

    // Create socket for Naming Server connection
    if ((nm_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed");
        return -1;
    }

    nm_addr.sin_family = AF_INET;
    nm_addr.sin_port = htons(NAMING_SERVER_PORT);

    if (inet_pton(AF_INET, NAMING_SERVER_IP, &nm_addr.sin_addr) <= 0)
    {
        perror("Invalid Naming Server address");
        return -1;
    }

    // Create Storage Server socket (for clients)
    if ((ss_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Storage socket creation failed");
        return -1;
    }

    ss_addr.sin_family = AF_INET;
    ss_addr.sin_addr.s_addr = INADDR_ANY;
    ss_addr.sin_port = 0; // Random available port

    if (bind(ss_sock, (struct sockaddr *)&ss_addr, sizeof(ss_addr)) < 0)
    {
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

    // Start listening
    if (listen(ss_sock, 5) < 0)
    {
        perror("Listen failed");
        return -1;
    }

    // Connect to Naming Server
    if (connect(nm_sock, (struct sockaddr *)&nm_addr, sizeof(nm_addr)) < 0)
    {
        perror("Connection to Naming Server failed");
        return -1;
    }

    cout << "[+] Connected to Naming Server (" 
         << NAMING_SERVER_IP << ":" << NAMING_SERVER_PORT << ")" << endl;

    string localIP = getLocalIP();
    cout << "[+] Local IP detected: " << localIP << endl;

    // --- Register with Naming Server ---
    string reg_msg = "Storage Server|PORT:" + to_string(assignedPort) + "|IP:" + localIP;
    send(nm_sock, reg_msg.c_str(), reg_msg.size(), 0);
    cout << "[→] Sent registration to Naming Server: " << reg_msg << endl;

    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = read(nm_sock, buffer, sizeof(buffer));
    if (bytesRead > 0)
        cout << "[←] Response from Naming Server: " << buffer << endl;
    else
        cout << "[!] No response or connection lost." << endl;

    cout << "[*] Storage Server is now registered and waiting for clients..." << endl;
    cout << "==============================\n";

    // --- Accept and handle client connections ---
    while (running)
    {
        socklen_t clientLen = sizeof(clientAddr);
        int clientSock = accept(ss_sock, (struct sockaddr *)&clientAddr, &clientLen);

        if (clientSock < 0)
        {
            perror("[!] Accept failed");
            continue;
        }

        string clientIP = inet_ntoa(clientAddr.sin_addr);
        int clientPort = ntohs(clientAddr.sin_port);
        cout << "\n[+] New client connected: " << clientIP << ":" << clientPort << endl;

        memset(buffer, 0, sizeof(buffer));
        ssize_t valread = read(clientSock, buffer, sizeof(buffer));

        if (valread > 0)
        {
            string clientMsg(buffer);
            cout << "← Received from client: " << clientMsg << endl;

            // Process the command
            string response = handleClientCommand(clientMsg);

            // Send response to client
            send(clientSock, response.c_str(), response.size(), 0);

            cout << "→ Sent to client: " << response << endl;
        }
        else
        {
            cout << "[!] Client disconnected or empty message." << endl;
        }

        close(clientSock);
        cout << "[*] Connection closed with client: " << clientIP << ":" << clientPort << endl;
    }

    cout << "[*] Storage Server shutting down..." << endl;
    close(nm_sock);
    close(ss_sock);
    return 0;
}
