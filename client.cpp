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
#include <list>
#include <unordered_map>

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
    string ownerID;
};

mutex lockMutex;
map<string, FileLock> fileLocks;

// -----------------------------------------
// LRU Cache (Filename -> Naming Server Response)
// -----------------------------------------
const int LRU_CAPACITY = 5;
list<string> lruList; // MRU at front
unordered_map<string, pair<string, list<string>::iterator>> lruCache;
mutex lruMutex;

bool getFromLRU(const string &key, string &value)
{
    lock_guard<mutex> guard(lruMutex);
    auto it = lruCache.find(key);
    if (it == lruCache.end())
        return false;

    lruList.erase(it->second.second);
    lruList.push_front(key);
    it->second.second = lruList.begin();
    value = it->second.first;
    return true;
}

void putIntoLRU(const string &key, const string &value)
{
    lock_guard<mutex> guard(lruMutex);

    if (lruCache.find(key) != lruCache.end())
    {
        lruList.erase(lruCache[key].second);
    }
    else if (lruCache.size() >= LRU_CAPACITY)
    {
        string lruKey = lruList.back();
        lruList.pop_back();
        lruCache.erase(lruKey);
    }

    lruList.push_front(key);
    lruCache[key] = {value, lruList.begin()};
}

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
        return "127.0.0.1";

    host_entry = gethostbyname(hostbuffer);
    if (!host_entry)
        return "127.0.0.1";

    IPbuffer = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));
    return string(IPbuffer);
}

// ---------------------------------------------------------------------------
// Query Naming Server WITH LRU
// ---------------------------------------------------------------------------
string queryNamingServer(const string &filename)
{
    string cached;
    if (getFromLRU(filename, cached))
    {
        cout << "[LRU HIT] " << filename << endl;
        return cached;
    }

    cout << "[LRU MISS] Querying Naming Server for " << filename << endl;
    string req = "LOOKUP|" + filename;
    send(nm_sock, req.c_str(), req.size(), 0);

    char buffer[1024] = {0};
    read(nm_sock, buffer, sizeof(buffer));
    string response(buffer);

    putIntoLRU(filename, response);
    return response;
}

// ---------------------------------------------------------------------------
// Function to handle client command
// ---------------------------------------------------------------------------
string handleClientCommand(const string &msg, int clientSock)
{
    stringstream ss(msg);
    string command, filename, data;

    getline(ss, command, '|');
    getline(ss, filename, '|');
    getline(ss, data, '|');

    if (command == "LOOKUP")
    {
        return queryNamingServer(filename);
    }

    if (command == "READ")
    {
        ifstream infile(filename);
        if (!infile)
            return "[ERROR] File not found";

        stringstream buffer;
        buffer << infile.rdbuf();
        infile.close();
        return buffer.str();
    }

    if (command == "WRITE")
    {
        sockaddr_in addr;
        socklen_t len = sizeof(addr);
        getpeername(clientSock, (sockaddr *)&addr, &len);
        string clientID =
            string(inet_ntoa(addr.sin_addr)) + ":" + to_string(ntohs(addr.sin_port));

        lockMutex.lock();
        auto &lockInfo = fileLocks[filename];

        if (lockInfo.locked && lockInfo.ownerID != clientID)
        {
            lockMutex.unlock();
            return "[ERROR] File locked by another client";
        }

        lockInfo.locked = true;
        lockInfo.ownerID = clientID;
        lockMutex.unlock();

        thread([filename, clientID]()
               {
                   this_thread::sleep_for(chrono::seconds(10));
                   lock_guard<mutex> g(lockMutex);
                   if (fileLocks[filename].ownerID == clientID)
                   {
                       fileLocks[filename].locked = false;
                       fileLocks[filename].ownerID = "";
                   }
               })
            .detach();

        ofstream outfile(filename, ios::trunc);
        if (!outfile)
            return "[ERROR] Cannot write file";

        outfile << data;
        outfile.close();
        return "[WRITE OK]";
    }

    if (command == "CREATE")
    {
        ofstream outfile(filename);
        if (!outfile)
            return "[ERROR] Cannot create file";
        outfile.close();
        return "[CREATE OK]";
    }

    return "[ERROR] Unknown command";
}

// ---------------------------------------------------------------------------
int main()
{
    signal(SIGINT, handleExit);

    sockaddr_in nm_addr{}, ss_addr{}, clientAddr{};
    char buffer[2048] = {0};

    nm_sock = socket(AF_INET, SOCK_STREAM, 0);
    nm_addr.sin_family = AF_INET;
    nm_addr.sin_port = htons(NAMING_SERVER_PORT);
    inet_pton(AF_INET, NAMING_SERVER_IP, &nm_addr.sin_addr);

    ss_sock = socket(AF_INET, SOCK_STREAM, 0);
    ss_addr.sin_family = AF_INET;
    ss_addr.sin_addr.s_addr = INADDR_ANY;
    ss_addr.sin_port = 0;

    bind(ss_sock, (sockaddr *)&ss_addr, sizeof(ss_addr));
    listen(ss_sock, 5);

    connect(nm_sock, (sockaddr *)&nm_addr, sizeof(nm_addr));

    string reg_msg = "Storage Server|IP:" + getLocalIP();
    send(nm_sock, reg_msg.c_str(), reg_msg.size(), 0);

    while (running)
    {
        socklen_t len = sizeof(clientAddr);
        int clientSock = accept(ss_sock, (sockaddr *)&clientAddr, &len);
        if (clientSock < 0)
            continue;

        memset(buffer, 0, sizeof(buffer));
        read(clientSock, buffer, sizeof(buffer));

        string response = handleClientCommand(buffer, clientSock);
        send(clientSock, response.c_str(), response.size(), 0);

        close(clientSock);
    }

    close(nm_sock);
    close(ss_sock);
    return 0;
}
