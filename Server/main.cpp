#include "../includes/ServerManager.hpp"
#include <signal.h>


void handler(int sig)
{
    if (sig == SIGINT)
    {
        std::cout << "\n*Webserv Quit Successfully*" << std::endl;
        exit(0);
    }
}


int main(int ac, char **av)
{
    signal(SIGINT, handler);
    std::string filePath;
    if (ac < 2)
        filePath = "default.conf";
    else if (ac == 2)
        filePath = av[1];
    else    
    {
        std::cerr << "Error: wrong number of arguments\n";
        return 1;
    }
    try
    {
        Server file;
        file.get_file(filePath);
        ServerManager server(file);
        server.initServers();
    }
    catch(std::exception& e)
    {
        std::cout << "Error: " << e.what()
            << std::endl;
        return 1;
    }
    return 0;
}