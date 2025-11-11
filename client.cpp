#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <unordered_map>
#include <arpa/inet.h>
#include <netdb.h>
using namespace std;

#define NAMING_SERVER_IP "127.0.0.1"
#define NAMING_SERVER_PORT 8080

// ---------------------------------------------------------------------------
// Helper function to connect to a given IP/port
// ---------------------------------------------------------------------------
int connectToServer(const string &ip, int port)
{
    int sock;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0)
    {
        perror("Invalid address / Address not supported");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Connection Failed");
        return -1;
    }

    return sock;
}

// ---------------------------------------------------------------------------
// Helper to get local IP (for handshake)
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
// Main
// ---------------------------------------------------------------------------
int main()
{
    int sock = 0;
    char buffer[1024] = {0};

    // Cache: filename → (IP, port)
    unordered_map<string, pair<string, int>> fileLocationCache;

    // Persistent storage server connections: (IP:PORT) → socket
    unordered_map<string, int> storageConnections;

    // Connect to Naming Server
    sock = connectToServer(NAMING_SERVER_IP, NAMING_SERVER_PORT);
    if (sock < 0)
        return -1;

    cout << "\n✅ Connected to Naming Server (" << NAMING_SERVER_IP << ":" << NAMING_SERVER_PORT << ")\n";

    string clientIP = getLocalIP();

    struct sockaddr_in localAddr;
    socklen_t addrLen = sizeof(localAddr);
    getsockname(sock, (struct sockaddr *)&localAddr, &addrLen);
    int clientPort = ntohs(localAddr.sin_port);

    string handshakeMsg = "Client|PORT:" + to_string(clientPort) + "|IP:" + clientIP;
    send(sock, handshakeMsg.c_str(), handshakeMsg.size(), 0);
    cout << "→ Sent handshake: " << handshakeMsg << endl;

    memset(buffer, 0, sizeof(buffer));
    read(sock, buffer, sizeof(buffer));
    cout << "← Received: " << buffer << endl;

    // -----------------------------------------------------------------------
    // Interactive Loop
    // -----------------------------------------------------------------------
    while (true)
    {
        cout << "\nChoose operation:\n";
        cout << "1. READ file\n";
        cout << "2. WRITE file\n";
        cout << "3. CREATE file\n";
        cout << "4. EXIT\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 4)
        {
            string exitMsg = "EXIT";
            send(sock, exitMsg.c_str(), exitMsg.size(), 0);
            cout << "[x] Exiting and closing all connections..." << endl;

            // Close all persistent storage connections
            for (auto &conn : storageConnections)
                close(conn.second);

            close(sock);
            return 0;
        }

        string op;
        if (choice == 1)
            op = "READ";
        else if (choice == 2)
            op = "WRITE";
        else if (choice == 3)
            op = "CREATE";
        else
        {
            cout << "[!] Invalid choice.\n";
            continue;
        }

        // --- Ask for filename first ---
        string filename;
        cout << "Enter filename: ";
        getline(cin, filename);

        // --- Ask for file data immediately if WRITE or CREATE ---
        string data = "";
        if (op == "WRITE" || op == "CREATE")
        {
            cout << "Enter file data: ";
            getline(cin, data);
        }

        // --- Check cache or ask Naming Server ---
        string ip;
        int port;

        if (fileLocationCache.find(filename) != fileLocationCache.end())
        {
            auto [cachedIP, cachedPort] = fileLocationCache[filename];
            ip = cachedIP;
            port = cachedPort;
            cout << "📦 Using cached location for '" << filename << "': " << ip << ":" << port << endl;
        }
        else
        {
            // No cache → ask Naming Server
            string namingReq = op + "|" + filename;
            send(sock, namingReq.c_str(), namingReq.size(), 0);
            cout << "→ Sent to Naming Server: " << namingReq << endl;

            memset(buffer, 0, sizeof(buffer));
            read(sock, buffer, sizeof(buffer));
            string response(buffer);
            cout << "← Naming Server Response: " << response << endl;

            // Expecting: STORAGE_SERVER|<port>|<ip>
            if (response.rfind("STORAGE_SERVER|", 0) == 0)
            {
                size_t firstSep = response.find("|", 15);
                if (firstSep == string::npos)
                {
                    cout << "[!] Malformed response from Naming Server.\n";
                    continue;
                }

                port = stoi(response.substr(15, firstSep - 15));
                ip = response.substr(firstSep + 1);

                // Cache it
                fileLocationCache[filename] = {ip, port};
                cout << "📦 Cached storage location for '" << filename << "' → " << ip << ":" << port << endl;
            }
            else
            {
                cout << "[!] Invalid response from Naming Server.\n";
                continue;
            }
        }

        // --- Persistent connection to Storage Server ---
        string serverKey = ip + ":" + to_string(port);
        int storageSock;

        if (storageConnections.find(serverKey) == storageConnections.end())
        {
            storageSock = connectToServer(ip, port);
            if (storageSock < 0)
            {
                cout << "[!] Failed to connect to Storage Server. Removing from cache.\n";
                fileLocationCache.erase(filename);
                continue;
            }
            storageConnections[serverKey] = storageSock;
            cout << "[+] Persistent connection established to " << serverKey << endl;
        }
        else
        {
            storageSock = storageConnections[serverKey];
        }

        // --- Now we already have file data, send request directly ---
        string msg;
        if (op == "WRITE" || op == "CREATE")
            msg = op + "|" + filename + "|" + data;
        else
            msg = op + "|" + filename;

        send(storageSock, msg.c_str(), msg.size(), 0);
        cout << "→ Sent to Storage Server: " << msg << endl;

        memset(buffer, 0, sizeof(buffer));
        ssize_t valread = read(storageSock, buffer, sizeof(buffer));
        if (valread <= 0)
        {
            cout << "[!] Lost connection to Storage Server " << serverKey << ". Removing from cache.\n";
            close(storageSock);
            storageConnections.erase(serverKey);
            fileLocationCache.erase(filename);
            continue;
        }

        cout << "← Response from Storage Server: " << buffer << endl;
    }

    return 0;
}
