typedef struct accessible_path{
    int storage_server_number;
    char path[MAX_LENGTH];
}accessible_path;

typedef struct SS_Info{
    char* ip;
    int NM_port;
    int client_port;
    
    // accessible_path paths_accessible[MAX_LENGTH];
}SS_Info;
