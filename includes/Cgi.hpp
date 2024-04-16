#ifndef CGI_HPP
# define CGI_HPP


#include "ServerManager.hpp"
#include <unistd.h>
#include "Location.hpp"
class ServerManager;
class Location;

class Cgi{
    private:
        pid_t           childProcess;
        char*           argv[10];
        char*           env[10];  
        int             fdGet;
        std::string     cgiFileName;
    public:
        clock_t     startTime;
        // std::string   tmpFileName;
        // bool checkExtensionScript(std::string);
        std::string getCgiFileName();
        void        serveCgi(Server& , int , Client&, std::string);
        void        executeFilePosted(Server& , int , Client&);
        void        setEnv(Client& , Location& location);
        void        fillParametresExecve(std::string path, Client& client);
        void        childProcessPost(Client&, Server& , int, std::string);
        void        childProcessGet(Client&, Server& , int);
        void        setCgiFileName(std::string fileName);
        void        hangChecker(Client& client, pid_t childProcess, std::string cgiFileName, Server& server);
        void        cgiResponse(Client& ,Server&, std::string&);
};




#endif