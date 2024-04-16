#ifndef SERVER_HPP
#define SERVER_HPP

#include <sstream>
#include <set>

#include "Location.hpp"
class Location;

//This is the class of configFile
class Server{
    public:
        /*main container*/  
        bool                        rootFlag;
        ssize_t                     max_body;   
        std::string                 root;
        std::string                 host;
        std::string                 uploadPath;
        std::vector<int>            listen;
        std::vector<Server>         servr;
        std::vector<Location>       locations;
        std::set<std::string>       duplicate_set;
        std::vector<std::string>    server_name;
        std::vector<std::string>    error_pages;
        std::vector<std::string>    duplicate_vec;

        Server();
        int     get_root(std::vector<std::string> word);
        int     get_listen(std::vector<std::string> word);
        int     get_host(std::vector<std::string> word);
        int     get_server_name(std::vector<std::string> word);
        int     get_error_page(std::vector<std::string> word);
        int     get_clienMAxBodySize(std::vector<std::string> word);
        int     get_uploadPath(std::vector<std::string> word);
        void    initializeArray();
        void    get_file(std::string);
        void    fill_server(std::string line, Server &srvr);
        
};



#endif