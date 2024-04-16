#include "../includes/Client.hpp"


std::string Cgi::getCgiFileName(){
    return this->cgiFileName;
}

void Cgi::setCgiFileName(std::string fileName){
    this->cgiFileName = fileName;
}

void Cgi::fillParametresExecve(std::string path, Client& client) {
    if (path.find(".php") != std::string::npos)
        this->argv[0] = strdup("bin/php-cgi");
    else 
        this->argv[0] = strdup("/usr/bin/python");
    this->argv[1] = strdup(path.c_str());
    this->argv[2] = NULL;

    int envIndex = 0;
    this->env[envIndex++] = strdup(("SCRIPT_FILENAME=" + path).c_str());
    this->env[envIndex++] = strdup(("PATH_INFO=" + path).c_str());
    this->env[envIndex++] = strdup("REDIRECT_STATUS=200");
    this->env[envIndex++] = strdup(("REQUEST_METHOD=" + client.method).c_str());
    this->env[envIndex++] = strdup(("CONTENT_TYPE=" + client.request.data["Content-Type"]).c_str());

    if (client.method == "POST") {
        int pos = client.path.rfind("/");
        std::string tmp = client.path.substr(pos + 1, client.path.length());
        this->env[envIndex++] = strdup(("SCRIPT_NAME=" + tmp).c_str());
        this->env[envIndex++] = strdup(("CONTENT_LENGTH=" + client.request.data["Content-Length"]).c_str());
    } 
    else if (client.method == "GET")
        this->env[envIndex++] = strdup(("QUERY_STRING=" + client.request.getQueryString()).c_str());
    if (client.request.data.find("Cookie") != client.request.data.end())
        this->env[envIndex++] = strdup(("HTTP_COOKIE=" + client.request.data["Cookie"]).c_str());
    this->env[envIndex] = NULL;
}



void errorFiles(std::string message, Client& client, Server& server, int LocationIndex){
    (void)message;
    client.response.throwErrorPage(server, LocationIndex, 500);
    client.CgiCopmpleted = true;
    client.postCompleted = true;
}

void Cgi::serveCgi(Server& server, int LocationIndex, Client& client, std::string absolutePath)
{
    (void)LocationIndex;
    if (!client.isForked)
    {
        client.isForked = true;
        if (access(absolutePath.c_str(), F_OK) != 0)
        {
                client.response.throwErrorPage(server, -1, 404);
                client.CgiCopmpleted = true;
                client.postCompleted = true;
                return ;
        }
        client.cgi.startTime = clock();
            std::stringstream ss;
            ss << client.socket;
            client.tmpFileName = "/tmp/tmp" + ss.str();
            client.tmpFileName += ".txt";
        client.cgi.childProcess = fork();
        if (client.cgi.childProcess == -1)
        {
            errorFiles("Fork Failed", client, server, client.response.getLocationIndex());
            client.CgiCopmpleted = true;
            client.postCompleted = true;
            return ;
        }
        else if (client.cgi.childProcess == 0)
        {
            if (client.method == "POST")
            {
                if (client.ifExtensionCgi(absolutePath)){
                    int fd = open(client.fileName.c_str(), O_RDONLY);
                    if (dup2(fd, 0) == -1){
                        errorFiles("Duping input Failed", client, server, client.response.getLocationIndex());
                        client.CgiCopmpleted = true;
                        client.postCompleted = true;
                        exit(1);
                    }
                    close(fd);
                }
                else {
                    int path = open(absolutePath.c_str(), O_WRONLY);
                    if (dup2(path, 1) == -1){
                        errorFiles("Duping input Failed", client, server, client.response.getLocationIndex());
                        client.CgiCopmpleted = true;
                        client.postCompleted = true;
                        exit(1);
                    }
                    close(path);
                }
            }
            FILE* tmp = freopen(client.tmpFileName.c_str(), "w", stdout);
            if (tmp == NULL){
                errorFiles("Duping input Failed", client, server, client.response.getLocationIndex());
                client.CgiCopmpleted = true;
                client.postCompleted = true;
                exit(1);
            }
            fillParametresExecve(absolutePath, client);
            execve(argv[0], argv, env);
        }
    }

    int pid = waitpid(client.cgi.childProcess, &client.status, WNOHANG);
    if (pid == 0)
    {
        client.cgiRunning = true;
        client.postCompleted = false;
        clock_t currentTime = clock();
        double time_used = static_cast<double>(currentTime - client.cgi.startTime) / CLOCKS_PER_SEC;
        if (time_used > 5)
        {
            kill(client.cgi.childProcess, SIGKILL);
            waitpid(client.cgi.childProcess, &client.status, 0);
            remove(client.tmpFileName.c_str());
            //!Remember to kill the zombie process
            if (client.method == "POST")
                remove(("uploads/" + cgiFileName).c_str());
            client.response.throwErrorPage(server, -1, 504);
            client.cgiRunning = false;
            client.postCompleted = true;
            client.CgiCopmpleted = true;
            return ;
        }
    }
    else if (pid > 0)
    {
        // to cheeeck later
        if (WIFEXITED(client.status))
            client.status = WEXITSTATUS(client.status);
        client.postCompleted = true;
        client.cgiRunning = false;
        this->cgiResponse(client, server, client.tmpFileName);
        remove(client.tmpFileName.c_str());
    }
    else
    {
        errorFiles("Error in waitpid", client, server, client.response.getLocationIndex());
        client.CgiCopmpleted = true;
        client.postCompleted = true;
        return ;
    }

}

void Cgi::cgiResponse(Client& client, Server& server, std::string &cgiFileName){
    std::ifstream file;
    file.open(cgiFileName);
    if (!file.is_open()){

        client.response.throwErrorPage(server, -1, 500);
        client.postCompleted = true;
        client.cgiRunning = false;
        return ;
    }
    std::string response;
    std::string line;
    response = "HTTP/1.1 200 OK\r\n";
    while(std::getline(file, line)){
            response += line+"\n";
    }
    client.response.setResponse(response);
    file.close();
}

