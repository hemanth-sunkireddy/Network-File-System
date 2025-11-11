#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <thread>
#include <map>
#include <vector>
#include <random>
#include <fstream>
#include <iomanip>

using namespace std;

#define PORT 8080

// Registry of storage servers: Port -> IP
map<int, string> storageServers;

// ==============================
// Trie Structure for File Lookup
// ==============================
struct TrieNode
{
    map<char, TrieNode *> children;
    bool isEndOfFile = false;
    string ip;
    int port = -1;
};

TrieNode *root = new TrieNode();

// Insert new file info
void insertFile(const string &filename, const string &ip, int port)
{
    TrieNode *node = root;
    for (char c : filename)
    {
        if (!node->children[c])
            node->children[c] = new TrieNode();
        node = node->children[c];
    }
    node->isEndOfFile = true;
    node->ip = ip;
    node->port = port;
}

// Search file info
pair<string, int> searchFile(const string &filename)
{
    TrieNode *node = root;
    for (char c : filename)
    {
        if (!node->children[c])
            return {"", -1};
        node = node->children[c];
    }
    if (node->isEndOfFile)
        return {node->ip, node->port};
    return {"", -1};
}

// ==============================
// Logging Function
// ==============================
void logToFile(const string &message)
{
    ofstream logFile("log.txt", ios::app);
    if (!logFile)
    {
        cerr << "[ERROR] Failed to open log file!" << endl;
        return;
    }

    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm *localTime = localtime(&now_c);

    logFile << "[" << put_time(localTime, "%Y-%m-%d %H:%M:%S") << "] "
            << message << endl;

    logFile.close();
}

// ---------------------------------------------------------------------------
// Monitor Storage Server
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
            logToFile("[x] Storage Server disconnected: " + clientIP + ":" + to_string(registeredPort));

            auto it = storageServers.find(registeredPort);
            if (it != storageServers.end() && it->second == clientIP)
            {
                storageServers.erase(it);
                cout << "[#] Removed from registry. Active servers: "
                     << storageServers.size() << endl;
            }

            close(socket_fd);
            return;
        }

        string msg(buffer);
        cout << "[Storage " << clientIP << "] Message: " << msg << endl;

        string ack = "ACK from Naming Server";
        send(socket_fd, ack.c_str(), ack.size(), 0);
    }
}

// ---------------------------------------------------------------------------
// Select Random Storage Server
// ---------------------------------------------------------------------------
pair<string, int> getRandomStorageServer()
{
    if (storageServers.empty())
        return {"", -1};

    vector<pair<int, string>> servers;
    for (auto &s : storageServers)
        servers.push_back({s.first, s.second});

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, servers.size() - 1);

    int index = dis(gen);
    int port = servers[index].first;
    string ip = servers[index].second;

    return {ip, port};
}

// ---------------------------------------------------------------------------
// Handle Client Requests
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
            logToFile("[x] Client disconnected unexpectedly: " + clientIP + ":" + to_string(clientPort));
            close(socket_fd);
            return;
        }

        string msg(buffer);
        cout << "[Client " << clientIP << " " << clientPort << "] Message: " << msg << endl;
        logToFile("[Client " + clientIP + ":" + to_string(clientPort) + "] Request: " + msg);

        if (msg == "EXIT")
        {
            cout << "[x] Client requested disconnection: " << clientIP << ":" << clientPort << endl;
            close(socket_fd);
            return;
        }

        size_t firstSep = msg.find('|');
        string op = msg.substr(0, firstSep);
        string filename = (firstSep != string::npos) ? msg.substr(firstSep + 1) : "";

        if (op == "READ" || op == "WRITE" || op == "CREATE")
        {
            if (storageServers.empty())
            {
                string err = "ERROR|No active storage servers available";
                send(socket_fd, err.c_str(), err.size(), 0);
                continue;
            }

            // --- Trie-based lookup ---
            auto [foundIP, foundPort] = searchFile(filename);

            if ((op == "READ" || op == "WRITE") && !foundIP.empty() && foundPort != -1)
            {
                string response = "STORAGE_SERVER|" + to_string(foundPort) + "|" + foundIP;
                send(socket_fd, response.c_str(), response.size(), 0);
                cout << "[→] Sent cached mapping for '" << filename << "' → " << foundIP << ":" << foundPort << endl;
                logToFile("[CACHE] Sent cached mapping for '" + filename + "' → " + foundIP + ":" + to_string(foundPort));
                continue;
            }

            // --- CREATE or first WRITE: random server ---
            auto [ip, port] = getRandomStorageServer();
            if (ip.empty() || port == -1)
            {
                string err = "ERROR|No available storage servers";
                send(socket_fd, err.c_str(), err.size(), 0);
                continue;
            }

            cout << "[#] Selected random Storage Server: " << ip << ":" << port << endl;

            if (op == "CREATE")
            {
                insertFile(filename, ip, port);
                cout << "[+] Registered new file '" << filename << "' on " << ip << ":" << port << endl;
                logToFile("[+] Registered new file '" + filename + "' on " + ip + ":" + to_string(port));
            }

            string response = "STORAGE_SERVER|" + to_string(port) + "|" + ip;
            send(socket_fd, response.c_str(), response.size(), 0);
            cout << "[→] Sent storage server info to client: " << response << endl;
            logToFile("[→] Sent response to Client " + clientIP + ":" + to_string(clientPort) + " → " + response);
        }
        else
        {
            const char *response = "UNKNOWN_COMMAND";
            send(socket_fd, response, strlen(response), 0);
        }
    }
}

// ---------------------------------------------------------------------------
// Handle Handshake
// ---------------------------------------------------------------------------
void handleConnection(int socket_fd, sockaddr_in client_addr)
{
    char buffer[1024] = {0};
    char clientIP_cstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP_cstr, INET_ADDRSTRLEN);
    string clientIP(clientIP_cstr);

    cout << "\n[+] Connection established from: " << clientIP
         << " | Port: " << ntohs(client_addr.sin_port) << endl;
    logToFile("[+] Connection established from: " + clientIP + " | Port: " + to_string(ntohs(client_addr.sin_port)));

    ssize_t bytesRead = read(socket_fd, buffer, sizeof(buffer));
    if (bytesRead <= 0)
    {
        cout << "[!] No handshake data received from " << clientIP << endl;
        close(socket_fd);
        return;
    }

    string msg(buffer);
    cout << "[Handshake]: " << msg << endl;

    if (msg.rfind("Storage Server", 0) == 0)
    {
        size_t portPos = msg.find("PORT:");
        size_t ipPos = msg.find("IP:");
        if (portPos != string::npos && ipPos != string::npos)
        {
            int port = stoi(msg.substr(portPos + 5, ipPos - portPos - 6));
            string ip = msg.substr(ipPos + 3);

            storageServers[port] = ip;
            cout << "[+] Registered Storage Server: " << ip << ":" << port << endl;
            logToFile("[+] Registered Storage Server: " + ip + ":" + to_string(port));

            cout << "[#] Total registered storage servers: " << storageServers.size() << endl;

            string ack = "Storage Registration Successful";
            send(socket_fd, ack.c_str(), ack.size(), 0);

            thread monitorThread(monitorStorageServer, socket_fd, ip, port);
            monitorThread.detach();
        }
    }
    else if (msg.rfind("Client", 0) == 0)
    {
        size_t portPos = msg.find("PORT:");
        size_t ipPos = msg.find("IP:");
        if (portPos != string::npos && ipPos != string::npos)
        {
            int clientPort = stoi(msg.substr(portPos + 5, ipPos - portPos - 6));
            string ip = msg.substr(ipPos + 3);

            cout << "[+] Registered Client: " << ip << ":" << clientPort << endl;
            logToFile("[+] Registered Client: " + ip + ":" + to_string(clientPort));

            string ack = "Client connected successfully!";
            send(socket_fd, ack.c_str(), ack.size(), 0);

            thread clientThread(handleClient, socket_fd, ip, clientPort);
            clientThread.detach();
        }
    }
    else
    {
        string err = "ERROR: Unknown connection type.";
        send(socket_fd, err.c_str(), err.size(), 0);
        close(socket_fd);
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

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket Creation Failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }

    char hostIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), hostIP, INET_ADDRSTRLEN);
    cout << "\n==============================\n";
    cout << "Naming Server started successfully!" << endl;
    cout << "Listening on IP: " << hostIP << " | Port: " << ntohs(address.sin_port) << endl;
    cout << "==============================\n";

    if (listen(server_fd, 10) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "[*] Waiting for incoming connections...\n";

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
