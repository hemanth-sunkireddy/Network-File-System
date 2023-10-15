#ifndef __HEADERS_H
#define __HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "server_receivingRequest.h"
#include "server_sendingResponse.h"
#include "client_sendingRequest.h"
#include "client_recievingResponse.h"

#define PORT 5000 
#define MAX_LENGTH 1024 


extern char *ip ;
extern int port;


#endif
