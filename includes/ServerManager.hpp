#pragma once


#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <fcntl.h>
#include <netdb.h>
#include "Client.hpp"
#include <map>
#include "Response.hpp"
#include <cstring>
#include <algorithm>
#include <dirent.h>
#include <signal.h>
#include "Response.hpp"
#include "Request.hpp"
#include <sys/stat.h>



#include "../includes/Config.hpp"
#include "../includes/webserv.hpp"

#define MAX_REQUEST_SIZE 4096
#define MAX_CHUNKED_SIZE 500
#define BUFFER_SIZE 1024
#define LOCATION this->servers[clientIter->serverID].locations[clientIter->response.getLocationIndex()]
#define MAP_DATA clientIter->request.data
#define CURR_SERVER this->servers[clientIter->serverID]
class Client;


class ServerManager
{
    private:
        std::vector<Server> servers;
        std::vector<int> serverSockets;
        fd_set readyToRead, readyToWrite;
        fd_set readyToReadCopy, readyToWriteCopy;
        int maxSocket;
        std::vector<Client> _clients;
        std::map<std::string, std::string>	types;
    public:
        ServerManager(const Server& config);
        ~ServerManager();
        void acceptIncommingConnection(int index);
        void manageClients(void);
        void setupServerSocket(const char *host, const char *port);
        void setSocketOption(int &Socket);
        void initServers();
        int setNonBlockingSocket(int &_socket);
        void print_error(std::string msg);
        void print_servers();
        void startServer();
        void waitClients();
        void PostCondition(std::vector<Client>::iterator& clientIter, Server&);
        void    sendResponse(Client &client);
        void    readRequestHeader(Client &client);
        void    checkForTimeout(std::vector<Client>::iterator& client);
        void    sendTimeoutResponse(Client &client);
        int     getServerId(Client &client);
        void    checkForServersValidity();
};
