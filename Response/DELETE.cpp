#include "../includes/ServerManager.hpp"
#include "../includes/Response.hpp"
#include <cstdio>


int Response::checkForPathToDelete(std::string &path, Server &clientServer, Client &client)
{
     (void)client;
    this->serverPath = this->getRealPath(clientServer, path);
    if (!pathExists(serverPath.c_str()))
        return 404;
    return 0;
}


bool Response::removeDirectory(const std::string& path)
{
    std::vector<std::string> fileList;
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(path.c_str())) == NULL)
        return false;
    while ((dirp = readdir(dp)) != NULL)
    {
        std::string fileName = std::string(dirp->d_name);
        if (fileName != "." && fileName != "..")
        {
            std::string filePath = path;
            if (filePath.at(filePath.length() - 1) != '/')
                filePath += "/";
            filePath += fileName;
            if (access(filePath.c_str(), W_OK) == 0)
            {
                fileList.push_back(filePath);
            }
        }
    }
    closedir(dp);
    for (size_t i = 0; i < fileList.size(); ++i)
    {
        if (isDirectory(fileList[i]))
        {
            if (!removeDirectory(fileList[i]))
                return false;
        }
        else
        {
            if (remove(fileList[i].c_str()) != 0)
                return false;
        }
    }
    if (rmdir(path.c_str()) != 0)
        return false;
    return true;
}

void    Response::deleteMethod(Client &client, Server &clientServer, std::string &path)
{
    int status = checkForPathToDelete(path, clientServer, client);
    if (status)
    {
        throwErrorPage(clientServer, this->LocationIndex, status);
        return ;
    }
    std::string pathToDelete = serverPath;
    if (isDirectory(pathToDelete))
    {
        if (pathToDelete.at(pathToDelete.length() - 1) != '/')
        {
            throwErrorPage(clientServer, this->LocationIndex, 409);
            return ;
        }
        if (!removeDirectory(pathToDelete))
        {
            throwErrorPage(clientServer, LocationIndex, 403);
            return ;
        }
    }
    else
    {
        if (access(pathToDelete.c_str(), W_OK) == 0)
        {
            if (remove(pathToDelete.c_str()))
            {
                throwErrorPage(clientServer, this->LocationIndex, 403);
                return ;
            }
        }
        else
        {
            throwErrorPage(clientServer, this->LocationIndex, 403);
            return ;
        }
    }
    this->headers = this->version + " 204 No Content\r\n";
    this->headers += "Connection: Close\r\n\r\n";
    this->response = headers;
}