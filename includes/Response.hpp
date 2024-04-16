#pragma once

#include "ServerManager.hpp"


class Client;
class Server;

class Response
{
    private:
        std::string response;
        bool    isMatched;
        std::string headers;
        std::string body;
        size_t  contentLen;
        std::string contentType;
        std::string method;
        char* buffer;
        std::string requestedResource;
        int status_code;
        bool    indexFileNotFound;
        int LocationIndex;
        std::string serverPath;
        std::string indexFile;
        std::string version;
        bool    isRedirected;
        bool    dirError;
        bool    isAlias;
        bool    deleteAccessError;
    public:
        Response();
        ~Response();
        int response_fd;
        bool    isFdOpen;
        void    buildResponse(Client &_client, Server clientServer);
        void    makeRedirectionRequest(std::string &path, Client &client);
        bool    matchLocations(std::string &path, Server &clientServer, Client _client);
        bool    isMethodAllowed(Server &srvr, int i);
        bool    serveIndexFile(Server &clientServer, Client &client);
        bool    serveFile(Server &clientServer, Client &client);
        void    throwErrorPage(Server &clientServer, int location, int errorCode);
        void    deleteMethod(Client &client, Server &clientServer, std::string &path);
        bool    pathExists(const char *path);
        int     checkForPathToDelete(std::string &path, Server &clientServer, Client &client);
        bool    isDirectory(std::string &path);
        bool    removeDirectory(const std::string& path);
        bool    notFromLocations(std::string &path, Server &server);
        bool    checkForIndexFile(Server &clientServer, int index);
        void    setResponse(std::string res);




        std::string     getAutoIndexPage(std::string &content);
        std::string     getHeaders();
        std::string     getResponse();
        bool    getFdStatus();
        void     getContentType();
        void    getMethod(Client &client, Server clientServer, std::string requestedResource);
        int    getRequestedResource(Server &clientServer, Client &client, std::string &requestedResource);
        int     getResponseFd();
        int getLocationIndex();
        size_t  getContentLen();
        std::string getRealPath(Server &srvr, std::string &requestedResource);
};