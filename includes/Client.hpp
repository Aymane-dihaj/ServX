#pragma once

#include "ServerManager.hpp"
#include <map>
#include "Request.hpp"
#include "Response.hpp"
#include "Cgi.hpp"
#include <sys/wait.h>

#define MAX_REQUEST_SIZE 4096

class Response;
class Request;
class Cgi;

class Client
{
    public:
        Client();

        int                             socket;
        int                             serverID;
        char	                        buffer[1024];
        bool                            socketError;
        bool                            isForked;
        bool                            sendingCompleted;
        bool                            sendHeaders;
        bool                            timerFlag;
        size_t                          received;
        ssize_t                         readBytes;
        socklen_t                       addressLenght;
        std::string                     method;
        std::string                     requestString;
        std::string                     path;
        std::string                     version;
        std::string                     content_type;
        std::string                     realPath;
        struct sockaddr_storage         clientAddress;
        clock_t                         _timeout;
        bool                            serverFound;
        clock_t                         startTime;
        Request                         request;
        Response                        response;
        bool                            cgiRunning;
        std::string                    tmpFileName;
        

                ///////////////////////////
                ////*---POST THINGS---*////
                ////  content-length   ////
        void            postContentLength(Client& client, Server&);
        void            postChuncked(Client &client, Server&);
        void            getContentType(std::string path);
        ssize_t         getFileSize(Client& client);
        std::string     generateRandomString(int length);
        std::string     createRandomStr(Client&client, Server&);
        std::string     getUploadSuccesPage();
        std::string     getNoContentPage();

        bool            headersParsed;
        bool            postCompleted;
        bool            fileCreated;
        bool            firstRead;
        bool            flagContntLength;
        size_t          count_bodyLen;
        size_t          content_length;
        std::string     randomString;
        std::string     fileName;
                ///   Chunked   ///
        bool            firstReadChunk;
        size_t          convertedHexaValue;
        size_t          counterChunk;
        std::string     strChunk;

                ///     CGI     ///
        Cgi             cgi;
        int             tmpFileCgi;
        int             ifExtensionCgi(std::string path);
        bool            CgiCopmpleted;
        bool            cgiError;
        bool            childFinish;
        int             status;
};