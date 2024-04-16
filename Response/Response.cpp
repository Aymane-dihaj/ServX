#include "../includes/ServerManager.hpp"
#include <sys/stat.h>

Response::Response()
{
    this->isFdOpen = false;
    this->response_fd = -1;
    this->isRedirected = false;
    this->status_code = -1;
    this->contentLen = 0;
    this->isMatched = true;
    this->deleteAccessError = false;
}

Response::~Response(){}

bool    Response::getFdStatus()
{
    return this->isFdOpen;
}

std::string Response::getRealPath(Server &srvr, std::string &requestedResource)
{
    std::string realPath;
    if (!srvr.locations[this->LocationIndex].alias.empty())
    {
        this->isAlias = true;
        if (srvr.locations[this->LocationIndex].alias.at(srvr.locations[this->LocationIndex].alias.size() - 1) == '/')
            srvr.locations[this->LocationIndex].alias.erase(srvr.locations[this->LocationIndex].alias.size() - 1);
        if (requestedResource.find(srvr.locations[this->LocationIndex].alias) != std::string::npos)
            requestedResource.erase(0, srvr.locations[this->LocationIndex].alias.size());
        size_t pos = requestedResource.find(srvr.locations[this->LocationIndex].NAME);
        if (pos != std::string::npos && srvr.locations[this->LocationIndex].NAME != "/")
            requestedResource.erase(pos, srvr.locations[this->LocationIndex].NAME.size());
        realPath = srvr.locations[this->LocationIndex].alias + requestedResource;
    }
    else
    {
        this->isAlias = false;
        if (srvr.locations[LocationIndex].root.size() == 0)
            srvr.locations[LocationIndex].root = srvr.root;
        if (srvr.locations[LocationIndex].root.at(srvr.locations[LocationIndex].root.size() - 1) == '/')
            srvr.locations[LocationIndex].root.erase(srvr.locations[LocationIndex].root.size() - 1);
        std::string locationRoot = srvr.locations[LocationIndex].root;
        int len = locationRoot.length();
        if (requestedResource.find(locationRoot) != std::string::npos)
            requestedResource.erase(0, len);
        realPath = locationRoot + requestedResource;
    }
    return realPath;
}

std::string getErrorPageName(int code)
{
    std::string path;
    if (code == 404)
        path = "404.html";
    else if (code == 400)
        path = "400.html";
    else if (code == 401)
        path = "401.html";
    else if (code == 403)
        path = "403.html";
    else if (code == 405)
        path = "405.html";
    else if (code == 409)
        path = "409.html";
    else if (code == 411)
        path = "411.html";
    else if (code == 413)
        path = "413.html";
    else if (code == 415)
        path = "415.html";
    else if (code == 500)
        path = "500.html";
    else if (code == 501)
        path = "501.html";
    else if (code == 502)
        path = "502.html";
    else if (code == 503)
        path = "503.html";
    else if (code == 504)
        path = "504.html";
    else if (code == 505)
        path = "505.html";
    else
        path = "400.html";
    return path;
}

std::string getMessage(int code)
{
    std::string message;
    if (code == 404)
        message = "Not Found";
    else if (code == 400)
        message = "Bad Request";
    else if (code == 401)
        message = "Unauthorized";
    else if (code == 403)
        message = "Forbidden";
    else if (code == 405)
        message = "Method Not Allowed";
    else if (code == 409)
        message = "Conflict";
    else if (code == 411)
        message = "Length Required";
    else if (code == 413)
        message = "Request Entity Too Large";
    else if (code == 415)
        message = "Unsupported Media Type";
    else if (code == 500)
        message = "Internal Server Error";
    else if (code == 501)
        message = "Not Implemented";
    else if (code == 502)
        message = "Bad Gateway";
    else if (code == 503)
        message = "Service Unavailable";
    else if (code == 504)
        message = "Gateway Timeout";
    else if (code == 505)
        message = "HTTP Version not supported";
    else
        message = "Bad Request";
    return message;
}

void    Response::throwErrorPage(Server &clientServer, int location, int errorCode)
{
    std::string pathToPage;
    if (location != -1)
    {
        if (clientServer.locations[location].error_pages.size() > 0)
        {
            for (unsigned int i = 0; i < clientServer.locations[location].error_pages.size(); i++)
            {
                if (std::atoi(clientServer.locations[location].error_pages[i].c_str()) == errorCode)
                {
                    pathToPage = clientServer.locations[location].error_pages[i + 1];
                    break;
                }
            }
        }
    }
    else if (clientServer.error_pages.size() > 0)
    {
        for (unsigned int i = 0; i < clientServer.error_pages.size(); i++)
        {
            if (std::atoi(clientServer.error_pages[i].c_str()) == errorCode)
            {
                pathToPage = clientServer.error_pages[i + 1];
                break;
            }
        }
    }
    std::ifstream infileCheck(pathToPage.c_str());
    bool isFileOpned = infileCheck.is_open();
    if (pathToPage.empty())
        pathToPage = "ErrorPages/" + getErrorPageName(errorCode);
    if (!isFileOpned)
        pathToPage = "ErrorPages/" + getErrorPageName(errorCode);
    infileCheck.close();
    std::ifstream infile(pathToPage.c_str());
    this->response_fd = open(pathToPage.c_str(), O_RDONLY);
    infile.seekg(0, std::ios::end);
    this->contentLen = infile.tellg();
    infile.seekg(0, std::ios::beg);
    infile.close();
    this->contentType = "text/html";
    this->status_code = errorCode;
    std::ostringstream oss;
    oss << this->status_code;
    this->headers += this->version + " ";
    std::string message = getMessage(errorCode);
    this->headers += oss.str();
    this->headers += " " + message + "\r\n";
    this->headers += "Content-Type: " + this->contentType;
    this->headers += "\r\n";
    std::ostringstream oss2;
    oss2 << this->contentLen;
    this->headers += "Content-Length: " + oss2.str();
    this->headers += "\r\n";
    this->headers += "Connection: close\r\n\r\n";
    this->response = this->headers;
    this->isFdOpen = true;
}


size_t  Response::getContentLen()
{
    return this->contentLen;
}

bool isDirectory(const char* path)
{
    struct stat info;

    if (stat(path, &info) != 0)
        return false;
    return S_ISDIR(info.st_mode);
}


std::string     Response::getResponse()
{
    return this->response;
}

std::string     Response::getHeaders()
{
    return this->headers;
}

int    Response::getResponseFd()
{
    return this->response_fd;
}

bool    Response::matchLocations(std::string &path, Server& clientServer, Client _client)
{
    (void)_client;
    (void)clientServer;
    std::string uriPath = path;
    while (uriPath != "")
    {
        for (size_t i = 0; i < clientServer.locations.size(); i++)
        {
            if (uriPath == clientServer.locations[i].NAME)
            {
                this->LocationIndex = i;
                return true;
            }
        }
        size_t index = uriPath.find_last_of("/");
        if (index == std::string::npos)
            break ;
        uriPath = uriPath.substr(0, index);
    }
    uriPath = "/";
    for (size_t i = 0; i < clientServer.locations.size(); i++)
    {
        if (uriPath == clientServer.locations[i].NAME)
        {
            this->LocationIndex = i;
            return true;
        }
    }
    if (this->LocationIndex == -1)
        return false;
    return false;
}

bool    Response::isMethodAllowed(Server &srvr, int i)
{
    if (srvr.locations[i].allowed_methods.size() > 0)
    {
        for (unsigned int index = 0; index < srvr.locations[i].allowed_methods.size(); index++)
        {
            if (this->method == srvr.locations[i].allowed_methods[index])
                return true;
        }
        return false;
    }
    else
    {
        if (this->method == "GET")
            return true;
        else
            return false;
    }
}


bool isAllowed(const std::string& input_string)
{
    std::string allowed_characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
    for (size_t i = 0; i < input_string.length(); ++i)
    {
        if (allowed_characters.find(input_string[i]) == std::string::npos)
            return true;
    }
    return false;
}
int    validRequest(Client &client)
{
    if (client.path.length() > 2048)
        return 414;
    if (client.path.empty() || client.path[0] != '/')
        return 400;
    if (client.version.empty())
        return 505;
    if (client.version != "HTTP/1.1")
        return 505;
    if (client.method != "POST" && client.method != "GET" && client.method != "DELETE")
        return 501;
    if (client.request.getLengthError())
        return 411;
    if (client.path.find("..") != std::string::npos)
        return 403;
    if (client.path.find(" ") != std::string::npos)
        return 400;
    if (client.path.find("\t") != std::string::npos)
        return 400;
    if (client.path.find("\r") != std::string::npos)
        return 400;
    if (client.path.find("\n") != std::string::npos)
        return 400;
    if (client.request.getRequestStatus())
        return 400;
    if (isAllowed(client.path))
        return 400;
    return 0;
}

int ServerManager::getServerId(Client &client)
{
    try
    {
        std::string host = client.request.data["Host"];
        host.erase(std::remove_if(host.begin(), host.end(), ::isspace), host.end());
        if (host.rfind('/') == std::string::npos && host.rfind(':') == std::string::npos)
        {
            for (size_t i = 0; i < this->servers.size(); i++)
            {
                for (size_t j = 0; j < this->servers[i].server_name.size(); j++)
                {
                    if (host == this->servers[i].server_name.at(j))
                    {
                        if (this->servers[client.serverID].listen.front() == this->servers[i].listen.front())
                            return i;
                    }
                }
            }
        }
        else
        {
            std::string port = host.substr(host.rfind(':') + 1);
            std::string serverName = host.substr(0, host.rfind(':'));
            for (size_t i = 0; i < this->servers.size(); i++)
            {
                for (size_t j = 0; j < this->servers[i].listen.size(); j++)
                {
                    if (std::stoi(port.c_str()) == this->servers[i].listen.at(j) && std::stoi(port.c_str()) == this->servers[client.serverID].listen.front() && this->servers[i].server_name.front() == serverName)
                        return i;
                }
            }
        }
    }
    catch(...)
    {
        return 0;
    }
    return (0);
}


void    Response::buildResponse(Client &_client, Server clientServer)
{
    this->requestedResource = _client.path;
    if (this->isMatched)
    {
        this->isMatched = false;
        this->method = _client.method;
        this->version = "HTTP/1.1";
        int x = validRequest(_client);
        if (x)
        {
            _client.request.setRequestStatus(true);
            _client.postCompleted = true;
            throwErrorPage(clientServer, -1, x);
            return ;
        }
        this->LocationIndex = -1;
        if (!matchLocations(requestedResource, clientServer, _client))
        {
            throwErrorPage(clientServer, -1, 404);
            return ;
        }
        else
        {
            if (!clientServer.locations[LocationIndex].redirections.empty())
            {
                this->status_code = std::atoi(clientServer.locations[LocationIndex].redirections[0].c_str());
                std::string redirectionCode = clientServer.locations[LocationIndex].redirections[0];
                if (status_code >= 300 && status_code < 400)
                {
                    serverPath = clientServer.locations[LocationIndex].redirections[1];
                    this->headers += this->version + " ";
                    this->headers += redirectionCode;
                    this->headers += " Moved Permanently\r\n";
                    if (serverPath.at(serverPath.length() - 1) != '/')
                        serverPath += "/";
                    this->headers += "Location: " + serverPath + "\r\n\r\n";
                    this->response += headers;
                }
                else
                {
                    std::ostringstream oss;
                    this->body = clientServer.locations[LocationIndex].redirections[1];
                    this->contentLen = body.size();
                    oss << this->contentLen;
                    this->headers += this->version + " ";
                    this->headers += redirectionCode + " OK\r\n";
                    this->headers += "Content-Length: ";
                    this->headers += oss.str();
                    this->headers += this->contentLen;
                    this->headers += "\r\n\r\n";
                    this->response += headers;
                    this->response += body;
                }
                return ;
            }
            if (!isMethodAllowed(clientServer, this->LocationIndex))
            {
                throwErrorPage(clientServer, this->LocationIndex, 405);
                _client.request.setRequestStatus(true);
                _client.postCompleted = true;
                return ;
            }
        }
    }
    if (_client.method == "GET")
        getMethod(_client, clientServer, requestedResource);
    if (_client.method == "DELETE")
        deleteMethod(_client, clientServer, requestedResource);
}



void    Response::setResponse(std::string res)
{
    this->response = res;
}


int Response::getLocationIndex(){
    return this->LocationIndex;
}
