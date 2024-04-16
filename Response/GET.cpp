#include "../includes/ServerManager.hpp"



bool Response::isDirectory(std::string &path)
{
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return false;
    return S_ISDIR(info.st_mode);
}

bool Response::pathExists(const char *path)
{
    return (access(path, F_OK) != -1);
}

bool isPathValid(const char *path)
{
    struct stat fileState;
    if (stat(path, &fileState) == 0)
    {
        if ((fileState.st_mode & S_IRUSR & S_IEXEC) != 0)
            return false;   
    }
    return true;
}

void Response::getContentType()
{
    std::string path = this->indexFile;
    try
    {
        path = path.erase(0, path.rfind('.'));
    }
    catch (...)
    {
        this->contentType = "application/octet-stream";
        return;
    }

    std::map<std::string, std::string> mimeTypes;
    mimeTypes.insert(std::make_pair(".css", "text/css"));
    mimeTypes.insert(std::make_pair(".php", "text/html"));
    mimeTypes.insert(std::make_pair(".csv", "text/csv"));
    mimeTypes.insert(std::make_pair(".gif", "image/gif"));
    mimeTypes.insert(std::make_pair(".htm", "text/html"));
    mimeTypes.insert(std::make_pair(".html", "text/html"));
    mimeTypes.insert(std::make_pair(".ico", "image/x-icon"));
    mimeTypes.insert(std::make_pair(".jpeg", "image/jpeg"));
    mimeTypes.insert(std::make_pair(".jpg", "image/jpeg"));
    mimeTypes.insert(std::make_pair(".js", "application/javascript"));
    mimeTypes.insert(std::make_pair(".json", "application/json"));
    mimeTypes.insert(std::make_pair(".pdf", "application/pdf"));
    mimeTypes.insert(std::make_pair(".png", "image/png"));
    mimeTypes.insert(std::make_pair(".svg", "image/svg+xml"));
    mimeTypes.insert(std::make_pair(".txt", "text/plain"));
    mimeTypes.insert(std::make_pair(".mp4", "video/mp4"));
    mimeTypes.insert(std::make_pair(".WebM", "video/webm"));
    mimeTypes.insert(std::make_pair(".Ogg", "video/ogg"));
    mimeTypes.insert(std::make_pair(".AVI", "video/x-msvideo"));
    mimeTypes.insert(std::make_pair(".MPEG", "video/mpeg"));
    mimeTypes.insert(std::make_pair(".tiff", "image/tiff"));
    mimeTypes.insert(std::make_pair(".tif", "image/tiff"));
    mimeTypes.insert(std::make_pair(".xml", "application/xml"));
    mimeTypes.insert(std::make_pair(".zip", "application/zip"));
    mimeTypes.insert(std::make_pair(".gz", "application/gzip"));
    mimeTypes.insert(std::make_pair(".tar", "application/x-tar"));
    mimeTypes.insert(std::make_pair(".rar", "application/x-rar-compressed"));
    mimeTypes.insert(std::make_pair(".7z", "application/x-7z-compressed"));
    mimeTypes.insert(std::make_pair(".mp3", "audio/mpeg"));
    mimeTypes.insert(std::make_pair(".wav", "audio/wav"));
    mimeTypes.insert(std::make_pair(".ogg", "audio/ogg"));
    mimeTypes.insert(std::make_pair(".flac", "audio/flac"));
    mimeTypes.insert(std::make_pair(".aac", "audio/aac"));
    mimeTypes.insert(std::make_pair(".mpga", "audio/mpeg"));
    mimeTypes.insert(std::make_pair(".mid", "audio/midi"));
    mimeTypes.insert(std::make_pair(".midi", "audio/midi"));
    mimeTypes.insert(std::make_pair(".ppt", "application/vnd.ms-powerpoint"));
    mimeTypes.insert(std::make_pair(".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"));
    mimeTypes.insert(std::make_pair(".xls", "application/vnd.ms-excel"));
    mimeTypes.insert(std::make_pair(".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
    mimeTypes.insert(std::make_pair(".doc", "application/msword"));
    mimeTypes.insert(std::make_pair(".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"));


    std::map<std::string, std::string>::iterator iter = mimeTypes.find(path);
    if (iter != mimeTypes.end())
        this->contentType = iter->second;
    else
        this->contentType = "application/octet-stream";
}

bool Response::serveFile(Server &clientServer, Client &client)
{
    (void)clientServer;
    (void)client;
    std::ifstream file;
    file.open(this->indexFile.c_str(), std::ios::binary);
    this->response_fd = open(indexFile.c_str(), O_RDWR);
    if (this->pathExists(this->indexFile.c_str()) == false)
        return false;
    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        this->contentLen = file.tellg();
        file.seekg(0, std::ios::beg);
        this->isFdOpen = true;
        this->status_code = 200;
        getContentType();
        file.close();
    }
    else
    {
        return false;
    }
    return true;
}


bool Response::serveIndexFile(Server &clientServer, Client &client)
{
    indexFileNotFound = false;
    (void)client;
    (void)clientServer;
    std::ifstream file;
    file.open(this->indexFile.c_str(), std::ios::binary);
    this->response_fd = open(indexFile.c_str(), O_RDWR);
    this->isFdOpen = true;
    if (this->pathExists(this->indexFile.c_str()) == false)
    {
        this->indexFileNotFound = true;
        return false;
    }
    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        this->contentLen = file.tellg();
        file.seekg(0, std::ios::beg);
        this->status_code = 200;
        this->serverPath = this->indexFile;
        getContentType();
        file.close();
    }
    else
    {
        return false;
    }
    return true;
}

void    Response::makeRedirectionRequest(std::string &path, Client &client)
{
    (void)client;
    this->headers = this->version + " 301 Moved Permanently\r\n";
    this->headers += "Location: " + path;
    this->headers += "\r\n\r\n";
    this->response += this->headers;
}

bool    Response::notFromLocations(std::string &path, Server &server)
{
    for (size_t i = 0; i < server.locations.size();i++)
    {
        if (server.locations[i].NAME == path)
            return false;
    }
    return true;
}

bool    Response::checkForIndexFile(Server &clientServer, int index)
{
    (void)clientServer;
    (void)index;
    DIR *dp;
    struct dirent *dir;
    dirError = true;

    dp = opendir(this->serverPath.c_str());
    if (dp == NULL)
    {
        closedir(dp);
        dirError = false;
        return false;
    }
    while ((dir = readdir(dp)) != NULL)
    {
        if (std::string(dir->d_name) == "index.html")
        {
            this->indexFile = this->serverPath + dir->d_name;
            closedir(dp);
            return true;
        }
    }
    closedir(dp);
    return false;
}


int Response::getRequestedResource(Server &clientServer, Client &client, std::string &requestedResource)
{
    this->serverPath = getRealPath(clientServer, requestedResource);
    if (!pathExists(serverPath.c_str()))
        return 404;
    else
    {
        if (isDirectory(serverPath))
        {
            if (serverPath.at(serverPath.size() - 1) != '/')
            {
                if (isAlias)
                {
                    serverPath.append("/");
                    makeRedirectionRequest(serverPath, client);
                }
                else
                {
                    requestedResource.append("/");
                    makeRedirectionRequest(requestedResource, client);
                }
                return 0;
            }
            if (checkForIndexFile(clientServer, this->LocationIndex))
            {
                if (!serveIndexFile(clientServer, client))
                    return 403;
                else
                    return 0;
            }
            else
            {
                if (!this->dirError)
                    return 403;
            }
            if (clientServer.locations[this->LocationIndex].index.size() > 0)
            {
                this->indexFile = clientServer.locations[LocationIndex].index.front();
                indexFile = this->serverPath += indexFile;
                if (clientServer.locations[this->LocationIndex].cgi && client.ifExtensionCgi(indexFile)){
                    client.cgi.serveCgi(clientServer, LocationIndex, client, indexFile);
                    return 0;
                }
                if (!serveIndexFile(clientServer, client))
                {
                    if (this->indexFileNotFound)
                        return 404;
                    return 403;
                }
                return 0;
            }
            if (clientServer.locations[LocationIndex].autoindex == true)
            {
                DIR *dir = opendir(serverPath.c_str());
                dirent *tmp;
                std::string content;
                std::string prev = "..";
            while ((tmp = readdir(dir)) != NULL)
            {
                if (std::string(tmp->d_name) == ".." || std::string(tmp->d_name) == ".")
                    continue ;
                content += "<li><a href=\"";
                if (std::string(tmp->d_name) != prev)
                    content += tmp->d_name;
                content += "\">";
                content += tmp->d_name;
                content += "</a></li>\n";
            }
            closedir(dir);
            this->contentType = "text/html";
            std::string result = getAutoIndexPage(content);
            this->body += result;
            }
            else
            {
                return 403;
            }
        }
        else
        {
            this->indexFile = serverPath;
            if (clientServer.locations[this->LocationIndex].cgi == true && client.ifExtensionCgi(indexFile)){
                client.cgi.serveCgi(clientServer, LocationIndex, client, indexFile);
                return 0;
            }
            else if (!serveFile(clientServer, client))
                return 403;
        }
    }
    return 0;
}


void    Response::getMethod(Client &client, Server clientServer, std::string requestedResource)
{
    int code = getRequestedResource(clientServer, client, requestedResource);
    if (code != 0)
    {
        throwErrorPage(clientServer, this->LocationIndex, code);
        return ;
    }

    if (this->response.size() == 0 && !client.cgiRunning)
    {
        if (status_code < 0)
            status_code = 200;
        char buffer[100];
        sprintf(buffer, "%s %d OK\r\n", this->version.c_str(), this->status_code);
        this->response += buffer;
        this->headers += buffer;
        if (this->contentType != "text/html")
        {
            sprintf(buffer, "Content-Type: %s\r\n", this->contentType.c_str());
            this->response += buffer;
            this->headers += buffer;
            sprintf(buffer, "Content-Length: %ld\r\n", this->contentLen);
            this->response += buffer;
            this->headers += buffer;
            sprintf(buffer, "Connection: keep-alive\r\n\r\n");
            this->response += buffer;
            this->headers += buffer; 
        }
        else
        {
            sprintf(buffer, "Content-Type: %s\r\n\r\n", this->contentType.c_str());
            this->response += buffer;
            this->headers += buffer;
        }
    }
    this->response += this->body;
    return ;
}
