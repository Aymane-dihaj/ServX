
#include "../includes/ServerManager.hpp"
#include "../includes/Request.hpp"


Request::~Request(){}

bool	Request::getLengthError()
{
	return this->lengthError;
}

Request::Request(){this->requestError = false;this->lengthError = false;}

std::string Request::getCookie()
{
	return this->cookie;
}

void	Request::setRequestStatus(bool status)
{
	this->requestError = status;
}

bool	Request::getRequestStatus()
{
	return this->requestError;
}

std::string Request::getQueryString()
{
	return this->queryString;
}

void	Request::parseRequest(Client &client)
{
	if (client.headersParsed == false)
	{
		client.requestString.append(client.buffer, client.readBytes);
		std::istringstream ss(client.requestString);
		ss >> client.method >> client.path >> client.version;
		size_t index = client.path.find("?");
		if (index != std::string::npos)
		{
			this->queryString = client.path.substr(index + 1, client.path.length());
			client.path = client.path.substr(0, index);
		}
		std::string line;
		while (getline(ss, line))
		{
			std::string name, value;
			std::istringstream iss(line);
			iss >> name >> value;
			if (name.size() > 3)
			{
				name.erase(name.size() - 1);
				this->data[name] = value;
			}
		}
		size_t pos = client.requestString.find("\r\n\r\n");
		if (pos == std::string::npos)
			return ;
		client.headersParsed = true;
		if (client.request.data.find("Host") == client.request.data.end())
		{
			client.postCompleted = true;
			this->requestError = true;
			return ;
		}
		client.requestString.erase(0, pos + 4);
		if (client.method != "POST")
			client.postCompleted = true;
		if (client.method != "POST" && client.requestString.size() > 0)
		{
			this->requestError = true;
			return ;
		}
		this->body += client.requestString;
		if (client.method == "POST" && this->data.find("Transfer-Encoding") == this->data.end() 
			&& this->data.find("Content-Length") == this->data.end())
		{
			client.postCompleted = true;
			this->lengthError = true;
			return ;
		}
	}
}