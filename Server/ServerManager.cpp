#include "../includes/ServerManager.hpp"


void ServerManager::print_error(std::string msg)
{
    std::cout << msg << std::endl;
    exit(EXIT_FAILURE);
}

ServerManager::ServerManager(const Server& config)
{
    this->servers = config.servr;
}
ServerManager::~ServerManager(){}


void ServerManager::setSocketOption(int &Socket)
{
    int opt = 1;
    if (setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        close(Socket);
        print_error("Failed to set socket option");
    }
}

int ServerManager::setNonBlockingSocket(int &_socket)
{
    if (fcntl(_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
    {
        return -1;
    }
    return 0;
}


void ServerManager::print_servers()
{
    for (size_t i = 0 ; i < this->servers.size(); i++)
    {
        std::cout << this->servers[i].server_name[0] << " is listening on ports: " << this->servers[i].listen.front();
        std::cout << std::endl;
    }
}

void ServerManager::acceptIncommingConnection(int serverIndex)
{
    if (FD_ISSET(serverSockets[serverIndex], &readyToReadCopy))
    {
        Client newClient;
        newClient.addressLenght = sizeof(newClient.clientAddress);
        newClient.socket = accept(serverSockets[serverIndex], (struct sockaddr*)&newClient.clientAddress, &newClient.addressLenght);
        if (newClient.socket == -1)
        {
            close(newClient.socket);
            return ;
        }
        setNonBlockingSocket(newClient.socket);
        FD_SET(newClient.socket, &readyToRead);
        FD_SET(newClient.socket, &readyToWrite);
        newClient._timeout = clock();
        newClient.serverID = serverIndex;
        _clients.push_back(newClient);
    }
}

void ServerManager::waitClients()
{
    std::vector<Client>::iterator iter = this->_clients.begin();
    while (iter != _clients.end())
    {
        if (iter->socket > maxSocket)
            maxSocket = iter->socket;
        iter++;
    }
    FD_ZERO(&readyToReadCopy);
    FD_ZERO(&readyToWriteCopy);
    readyToReadCopy = readyToRead;
    readyToWriteCopy = readyToWrite;
    if (select(maxSocket + 1, &readyToReadCopy, &readyToWriteCopy, 0, 0) == -1)
        return ;
}


void    ServerManager::sendResponse(Client &client)
{
    if (FD_ISSET(client.socket, &this->readyToWriteCopy))
    {
        std::string res = client.response.getResponse();
        int i = send(client.socket, res.c_str(), res.size(), 0);
        if (i < 1)
            client.socketError = false;
        else
        {
            client.sendingCompleted = true;
            client.sendHeaders = true;
            client.socketError = true;
        }
    }
}

 
void    ServerManager::readRequestHeader(Client &client)
{
    client.readBytes = read(client.socket, client.buffer, BUFFER_SIZE);
    if (client.readBytes < 0)
        client.socketError = false;
    else if (client.readBytes == 0)
    {
        FD_CLR(client.socket, &readyToRead);
        client.socketError = true;
    }
    else
        client.socketError = true;
}

void    ServerManager::PostCondition(std::vector<Client>::iterator& clientIter, Server& server)
{
    std::string resource = clientIter->path;

    clientIter->realPath = clientIter->response.getRealPath(server, resource);
    if (!clientIter->response.pathExists(clientIter->realPath.c_str()) && clientIter->postCompleted == false)
    {
        clientIter->response.throwErrorPage(server, clientIter->response.getLocationIndex(), 404);
        clientIter->postCompleted = true;
    }
    else if(!clientIter->request.getRequestStatus() && clientIter->method == "POST" && clientIter->postCompleted == false)
    {
        if (MAP_DATA.find("Transfer-Encoding") != MAP_DATA.end())
        {
            if (clientIter->requestString == "0\r\n\r\n"){
                clientIter->postCompleted = true;
                clientIter->response.throwErrorPage(this->servers[clientIter->serverID], -1, 400);
                return ;
            }
            else if (MAP_DATA["Transfer-Encoding"] == "chunked"){
                clientIter->postChuncked(*clientIter, server);
            }
            else
            {
                clientIter->response.throwErrorPage(CURR_SERVER,-1, 501);
                clientIter->postCompleted = true;
            }
        }
        else if(MAP_DATA.find("Content-Length") != MAP_DATA.end())
                clientIter->postContentLength(*clientIter, server);
        else
        {
            clientIter->response.throwErrorPage(CURR_SERVER, -1, 400);
            clientIter->postCompleted = true;
        }
    }
}

void    ServerManager::sendTimeoutResponse(Client &client)
{
    std::string response = "HTTP/1.1 408 Request Timeout\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += "<html><body><h1>408 Request Timeout</h1></body></html>";
    if(FD_ISSET(client.socket, &readyToWriteCopy))
        send(client.socket, response.c_str(), response.size(), 0);
}

void    ServerManager::checkForTimeout(std::vector<Client>::iterator& client)
{
        clock_t currentTime = clock();
        double elapsedTime = (double) (currentTime - client->_timeout) / CLOCKS_PER_SEC;
        if (elapsedTime >= 10)
        {
            if (client->method == "POST")
                remove(client->fileName.c_str());
            sendTimeoutResponse(*client);
            FD_CLR(client->socket, &readyToRead);
            FD_CLR(client->socket, &readyToWrite);
            close(client->socket);
            client = _clients.erase(client);
        }
        return ;
}

void ServerManager::manageClients(void)
{
    signal(SIGPIPE, SIG_IGN);
    std::vector<Client>::iterator clientIter = _clients.begin();
    while (clientIter != _clients.end())
    {
        if (FD_ISSET(clientIter->socket, &readyToReadCopy))
        {
            clientIter->_timeout = clock();
            readRequestHeader(*clientIter);
            if (!clientIter->socketError)
            {
                FD_CLR(clientIter->socket, &readyToRead);
                FD_CLR(clientIter->socket, &readyToWrite);
                close(clientIter->socket);
                clientIter = _clients.erase(clientIter);
                continue ;
            }
            clientIter->request.parseRequest(*clientIter);
            if(clientIter->headersParsed)
            {
                if (!clientIter->serverFound)
                    clientIter->serverID = this->getServerId(*clientIter);
                clientIter->response.buildResponse(*clientIter, CURR_SERVER);
            }
            if (!clientIter->request.getRequestStatus() && clientIter->method == "POST")
                PostCondition(clientIter, CURR_SERVER);
            else
            {
                FD_CLR(clientIter->socket, &readyToRead);
            }
            
        }
        else if (FD_ISSET(clientIter->socket, &readyToWriteCopy) && clientIter->headersParsed && clientIter->postCompleted)
        {
            clientIter->_timeout = clock();
            if (!clientIter->sendHeaders)
                sendResponse(*clientIter);
            if (!clientIter->socketError)
            {
                FD_CLR(clientIter->socket, &readyToWrite);
                FD_CLR(clientIter->socket, &readyToRead);
                close(clientIter->socket);
                if (clientIter->response.isFdOpen)
                    close(clientIter->response.response_fd);
                clientIter = _clients.erase(clientIter);
                continue ;
            }
            if (clientIter->response.isFdOpen)
            {
                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));
                int ret = read(clientIter->response.response_fd, buffer, 1024);
                if (ret < 1)
                {
                    clientIter->sendingCompleted = true;
                }
                else
                {
                    int s = send(clientIter->socket, buffer, ret, 0);
                    if (s < 1)
                    {
                        FD_CLR(clientIter->socket, &readyToWrite);
                        FD_CLR(clientIter->socket, &readyToRead);
                        close(clientIter->socket);
                        close (clientIter->response.response_fd);
                        clientIter = _clients.erase(clientIter);
                        continue ;   
                    }
                    clientIter->received += s;
                    if (clientIter->received >= clientIter->response.getContentLen())
                        clientIter->sendingCompleted = true;
                    else
                        clientIter->sendingCompleted = false;
                }
            }
            if (clientIter < _clients.end() && clientIter->sendingCompleted)
            {
                if (clientIter->response.isFdOpen)
                {
                    close(clientIter->response.response_fd);
                    clientIter->response.isFdOpen = false;
                }
                FD_CLR(clientIter->socket, &readyToWrite);
                FD_CLR(clientIter->socket, &readyToRead);
                close(clientIter->socket);
                clientIter = _clients.erase(clientIter);
                continue ;
            }
        }
        else if (clientIter->cgiRunning)
        {
            try{
                clientIter->cgi.serveCgi(CURR_SERVER, clientIter->response.getLocationIndex(), *clientIter, "");
            }
            catch(...)
            {
                clientIter->cgiRunning = false;
            }
        }
        else
            checkForTimeout(clientIter);
        if (clientIter < _clients.end())
            clientIter++;
    }
}


void ServerManager::startServer()
{
    FD_ZERO(&(this->readyToRead));
    FD_ZERO(&(this->readyToWrite));
    maxSocket = serverSockets.front();
    for (size_t i = 0; i < serverSockets.size(); ++i)
    {
        FD_SET(serverSockets[i], &readyToRead);
        if (serverSockets[i] > maxSocket)
            maxSocket = serverSockets[i];
    }
    while (true)
    {
        waitClients();
        for (size_t i = 0; i < serverSockets.size(); ++i)
            acceptIncommingConnection(i);
        manageClients();
    }
    for (size_t i = 0; i < serverSockets.size(); ++i)
        close(serverSockets[i]);
}

void ServerManager::checkForServersValidity()
{
    for (size_t i = 0; i < this->servers.size(); ++i)
    {
        for (size_t j = i + 1; j < this->servers.size(); ++j)
        {
            if (this->servers[i].server_name == this->servers[j].server_name &&
                this->servers[i].listen == this->servers[j].listen)
            {
                print_error("Error: Duplicate server with same port and server name");
            }
        }
    }
}

void ServerManager::setupServerSocket(const char *host, const char *port)
{
    checkForServersValidity();
    struct addrinfo hints, *data;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int result = getaddrinfo(host, port, &hints, &data);
    if (result)
        print_error("Error: getaddinfo failed\n");
    int sock = socket(data->ai_family, data->ai_socktype, data->ai_protocol);
    if (sock == -1)
        print_error("Error: Socket failed\n");
    this->serverSockets.push_back(sock);
    setSocketOption(serverSockets.back());
    setNonBlockingSocket(serverSockets.back());
    if (bind(serverSockets.back(), data->ai_addr, data->ai_addrlen ))
        print_error("Error: Failed to bind.");
    if (listen(serverSockets.back(), 0))
        print_error("Error: listen failed");
    freeaddrinfo(data);
}


void ServerManager::initServers(void)
{
    std::vector<int> used_ports;
    for(unsigned int i = 0; i < this->servers.size(); ++i)
    {
        if (this->servers[i].listen.size() >= 1)
        {
            if (std::find(used_ports.begin(), used_ports.end(), servers[i].listen.front()) == used_ports.end())
            {
                std::stringstream tmpPort;
                tmpPort << servers[i].listen.front();
                setupServerSocket(servers[i].host.c_str(), tmpPort.str().c_str());
                used_ports.push_back(servers[i].listen.front());
                std::cout << "Server: " << i << " is listening on: " << servers[i].listen.front() << std::endl;
            }
        }
        else
            print_error("Error: server must contain at least one port");
    }
    startServer();
}