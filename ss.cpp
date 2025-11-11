#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <csignal>
#include <netdb.h>
#include <fstream>
#include <sstream>
#include <mutex>
#include <map>
#include <thread>
#include <chrono>
using namespace std;

#define NAMING_SERVER_IP "127.0.0.1"
#define NAMING_SERVER_PORT 8080

int nm_sock = 0; // Naming Server socket
int ss_sock = 0; // Storage Server's listening socket
bool running = true;

// -----------------------------------------
// File Lock Table
// -----------------------------------------
struct FileLock
{
    bool locked = false;
    string ownerID; // which client owns this lock
};

std::mutex lockMutex;
std::map<std::string, FileLock> fileLocks;

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
string handleClientCommand(const string &msg, int clientSock)
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
        // --- Get client IP and port (unique ID) ---
        sockaddr_in addr;
        socklen_t len = sizeof(addr);
        getpeername(clientSock, (sockaddr *)&addr, &len);
        string clientIP = string(inet_ntoa(addr.sin_addr));
        int clientPort = ntohs(addr.sin_port);
        string clientID = clientIP + ":" + to_string(clientPort);

        // --- Lock logic ---
        lockMutex.lock();
        auto &lockInfo = fileLocks[filename];

        // If file already locked by another client → reject
        if (lockInfo.locked && lockInfo.ownerID != clientID)
        {
            lockMutex.unlock();
            return "[ERROR] Another client (" + lockInfo.ownerID + ") is currently writing to " + filename;
        }

        // Otherwise, lock it for this client
        lockInfo.locked = true;
        lockInfo.ownerID = clientID;
        lockMutex.unlock();

        cout << "[*] File '" << filename << "' locked by client " << clientID << " for 10 seconds." << endl;

        // --- Background unlock timer (non-blocking) ---
        std::thread([filename, clientID]()
                    {
        std::this_thread::sleep_for(std::chrono::seconds(10));

        lockMutex.lock();
        auto it = fileLocks.find(filename);
        if (it != fileLocks.end() && it->second.locked && it->second.ownerID == clientID)
        {
            it->second.locked = false;
            it->second.ownerID = "";
            cout << "[*] Auto-unlocked file: " << filename << " after 10 seconds." << endl;
        }
        lockMutex.unlock(); })
            .detach();

        // --- Perform write ---
        ofstream outfile(filename, ios::trunc);
        if (!outfile)
        {
            lockMutex.lock();
            auto &info = fileLocks[filename];
            info.locked = false;
            info.ownerID = "";
            lockMutex.unlock();
            return "[ERROR] Cannot open file for writing: " + filename;
        }

        outfile << data;
        outfile.close();

        cout << "[WRITE] Data written to file: " << filename << " by client " << clientID << endl;
        return "[WRITE OK] File locked by this client and data written to " + filename;
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

int main()
{
    signal(SIGINT, handleExit);

    struct sockaddr_in nm_addr, ss_addr, clientAddr;
    char buffer[2048] = {0};

    // --- Create socket for Naming Server connection ---
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

    // --- Create Storage Server socket (for clients) ---
    if ((ss_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Storage socket creation failed");
        return -1;
    }

    ss_addr.sin_family = AF_INET;
    ss_addr.sin_addr.s_addr = INADDR_ANY;
    ss_addr.sin_port = 0; // Let OS pick a random available port

    if (bind(ss_sock, (struct sockaddr *)&ss_addr, sizeof(ss_addr)) < 0)
    {
        perror("Bind failed");
        return -1;
    }

    // --- Get assigned random port ---
    socklen_t addrlen = sizeof(ss_addr);
    getsockname(ss_sock, (struct sockaddr *)&ss_addr, &addrlen);
    int assignedPort = ntohs(ss_addr.sin_port);

    cout << "\n==============================\n";
    cout << "[+] Storage Server started" << endl;
    cout << "[+] Assigned random port: " << assignedPort << endl;

    // --- Start listening for client connections ---
    if (listen(ss_sock, 5) < 0)
    {
        perror("Listen failed");
        return -1;
    }

    // --- Connect to Naming Server ---
    if (connect(nm_sock, (struct sockaddr *)&nm_addr, sizeof(nm_addr)) < 0)
    {
        perror("Connection to Naming Server failed");
        return -1;
    }

    cout << "[+] Connected to Naming Server (" << NAMING_SERVER_IP << ":" << NAMING_SERVER_PORT << ")" << endl;

    string localIP = getLocalIP();
    cout << "[+] Local IP detected: " << localIP << endl;

    // --- Register this Storage Server with the Naming Server ---
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

    // --- Main client handling loop ---
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
        string clientID = clientIP + ":" + to_string(clientPort);

        cout << "\n[+] New client connected: " << clientID << endl;

        memset(buffer, 0, sizeof(buffer));
        ssize_t valread = read(clientSock, buffer, sizeof(buffer));

        if (valread > 0)
        {
            string clientMsg(buffer);
            cout << "← Received from client: " << clientMsg << endl;

            // --- Process the command ---
            string response = handleClientCommand(clientMsg, clientSock);

            // --- Send response back ---
            send(clientSock, response.c_str(), response.size(), 0);
            cout << "→ Sent to client: " << response << endl;
        }
        else
        {
            cout << "[!] Client disconnected or empty message." << endl;
        }

        // --- Release any locks owned by this client ---
        // lockMutex.lock();
        // for (auto &entry : fileLocks)
        // {
        //     if (entry.second.locked && entry.second.ownerID == clientID)
        //     {
        //         entry.second.locked = false;
        //         entry.second.ownerID = "";
        //         cout << "[*] Released lock for file: " << entry.first << " (client " << clientID << ")" << endl;
        //     }
        // }
        // lockMutex.unlock();

        close(clientSock);
        cout << "[*] Connection closed with client: " << clientID << endl;
    }

    // --- Shutdown cleanup ---
    cout << "[*] Storage Server shutting down..." << endl;
    close(nm_sock);
    close(ss_sock);
    return 0;
}
