#pragma once

#include "ServerManager.hpp"

class Client;

class Request
{
    private:
        bool        requestError;
        std::string queryString;
        std::string cookie;
        bool    lengthError;
    public:
        Request();
        ~Request();
        
        std::string body;
        std::map<std::string, std::string> data;

        bool        getRequestStatus();
        void        setRequestStatus(bool status);
        void        parseRequest(Client &client);
        std::string getQueryString();
        std::string getCookie();
        bool        getLengthError();
};

