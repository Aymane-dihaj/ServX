#include "../includes/ServerManager.hpp"

Client::Client()
{
    this->socketError = true;
    memset(buffer, 0, 1025);
    received = 0;
    postCompleted = false;
    headersParsed = false;
    firstRead = false;
    readBytes = 0;
    content_length = 0;
    fileCreated = false;
    firstReadChunk = false;
    counterChunk = false;
    postCompleted = false;
    received = 0;
    sendHeaders = false;
    CgiCopmpleted = false;
    serverFound = false;
    timerFlag = false;
    status = -1;
    isForked = false;
    childFinish = false;
    cgiRunning = false;
}

int Client::ifExtensionCgi(std::string path){
    size_t posDot = path.rfind(".");
    if (posDot != std ::string::npos){
        std::string extension = path.substr(posDot, path.length());
        if (extension == ".php")
            return 1;
        if (extension == ".py")
            return 2;
    }
    return 0;
}