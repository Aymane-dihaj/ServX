#include "../includes/webserv.hpp"

void removeSpaces(std::string& line){
     line.erase(remove_if(line.begin(), line.end(), isspace),\
         line.end());
 }

Server::Server(){
    this->host = "";
    this->max_body = 10737418240;
    this->rootFlag = false;
}

void BracketsCheck(std::string configfile){
    std::ifstream ss;
    ss.open(configfile.c_str());
     if (!ss.is_open())
        throw std::invalid_argument("can't open file");
    
    int i = 0;
    char c;
    std::stack<char> stack;
    
    while(ss.get(c)){
        if (c == '{'){
            stack.push(c);
            i++;
        }
        else if (c == '}'){
            if (stack.empty()) 
                throw std::invalid_argument("Brackets");
            else
                stack.pop();
        }
    }
    if (i == 0)
        throw std::invalid_argument("There is no brackets!!");
    if (!stack.empty())
        throw std::invalid_argument("Brackets");
    ss.close();
}

std::vector<std::string> split_line(std::string line, std::string delimiter){
    size_t start = 0;
    size_t end;
    std::string tmp;
    std::vector<std::string> res;
    while ((end = line.find(delimiter, start)) != std::string::npos){
        tmp = line.substr(start, end - start);
        if (!tmp.empty())
            res.push_back(tmp);
        start = end + delimiter.length();
    }
    tmp = line.substr(start);
    if(!tmp.empty())
        res.push_back(tmp);
    return res;
}

int check_is_digit(std::string word){
    for (int i = 0; word[i]; i++){
        if (!isdigit(word[i]))
            return 0;
    }
    return 1;
}

void Server::initializeArray(){

}

int Server::get_listen(std::vector<std::string> word){
    if ("listen" == word[0]){
        for (size_t i = 1; i < word.size(); i++){
            if (!check_is_digit(word[i]))
                throw std::invalid_argument("listen: invalid character");
            if (std::atoi(word[i].c_str()) > 65535)
                throw std::invalid_argument("listen: value out of range");
            this->listen.push_back(std::atoi(word[i].c_str()));
        }
        return 1;
    }
    return 0;
}

int Server::get_host(std::vector<std::string> word){
    if ("host" == word[0]){
        if (word.size() != 2)
            throw std::invalid_argument("host : invalid input");
        int count = 0;
        for(size_t i = 0; i < word[1].length(); i++){
            if(word[1][i] == '.')
                count++;
        }
        if (count != 3)
            throw std::invalid_argument("host: error '.'");
        std::vector<std::string> ip_add = split_line(word[1], ".");
        if (ip_add.size() != 4)
            throw std::invalid_argument("host: invalid size");
        for (size_t i = 0;i < ip_add.size(); i++){
            if(!check_is_digit(ip_add[i]))
                throw std::invalid_argument("host: invalid character");
            if (ip_add[i].length() > 3)
                throw std::invalid_argument("host: invalid range");
            if (std::atoi(ip_add[i].c_str()) > 255 || std::atoi(ip_add[i].c_str()) < 0)
                throw std::invalid_argument("host: invalid range!");
        }
        this->host = word[1];
        return 1;
    }
    return 0;
}

int Server::get_server_name(std::vector<std::string> word){
    if ("server_name" == word[0]){
        if (word.size() < 2)
            throw std::invalid_argument("Server: server_name: invalid input!");
        std::set<std::string> dupp;
        for(size_t i = 1; i < word.size(); i++){
            this->server_name.push_back(word[i]);
            dupp.insert(word[i]);
        }
        if (dupp.size() != this->server_name.size())
            throw std::invalid_argument("Server :duplicate server name");
        return 1;
    }
    return 0;
}

int Server::get_error_page(std::vector<std::string> word){
    if ("error_page" == word[0]){
        if (word.size() != 3)
            throw std::invalid_argument("location: error_pages: missing code or target..!");
        if (word[1].length() != 3)
            throw std::invalid_argument("location: error_pages: invalid code");
        for (size_t i = 1; i < word.size(); i++)
            this->error_pages.push_back(word[i]);
        return 1;
    }
    return 0;
}




int Server::get_clienMAxBodySize(std::vector<std::string> word){
    if ("client_max_body_size" == word[0]){
        if (word.size() != 2)
            throw std::invalid_argument("server: client_max_body_size: invalid input");
        for (size_t i = 0; i < word[1].length(); i++)
            if (!std::isdigit(word[1][i]))
                throw std::invalid_argument("server: client_max_body_size: invalid input");
        std::stringstream ss(word[1]);
        ss >> this->max_body;
        if (this->max_body > 10737418240)
                throw std::invalid_argument("server: client_max_body_size: invalid input");
        return 1;
    }
    return 0;
}

int Server::get_root(std::vector<std::string> word){
    if (word[0] == "root"){

        if(word.size() != 2)
            throw std::invalid_argument("Server: root: invalid input");
        this->root = word[1];
        return 1;
    }
    return 0;
}

int Server::get_uploadPath(std::vector<std::string> word){
    if(word[0] == "upload_path"){
        if (word.size() != 2)
            throw std::invalid_argument("Server: upload_path: invalid input");
        this->uploadPath = word[1];
        return 1;
    }
    return 0;
}

void Server::fill_server(std::string line, Server &srvr){
    size_t posVergul = line.find(';');
    line.erase(posVergul);
    std::vector<std::string> word = split_line(line, " ");
    if (word.size() < 2)
        throw std::invalid_argument("server: invalid input");
    else if (posVergul == std::string::npos && word[0] != "}")
        throw std::invalid_argument("can't find ';'");
    if (word[0] != "listen" && word[0] != "host" && word[0] != "server_name"\
        && word[0] != "error_page" && word[0] != "client_max_body_size"\
        && word[0] != "root" && word[0] != "upload_path")
        throw std::invalid_argument("server: invalid input in the server context!!!");
    else if(srvr.get_listen(word)){
        srvr.duplicate_set.insert("listen");
        srvr.duplicate_vec.push_back("listen");
    }
    else if(srvr.get_host(word)){
        srvr.duplicate_set.insert("host");
        srvr.duplicate_vec.push_back("host");
    }
    else if(srvr.get_server_name(word)){
        srvr.duplicate_set.insert("server_name");
        srvr.duplicate_vec.push_back("server_name");
    }
    else if(srvr.get_root(word)){
        srvr.duplicate_set.insert("root");
        srvr.duplicate_vec.push_back("root");
        srvr.rootFlag = true;
    }
    else if(srvr.get_clienMAxBodySize(word)){
        srvr.duplicate_set.insert("client_max_body_size");
        srvr.duplicate_vec.push_back("client_max_body_size");
    }
    else if(srvr.get_uploadPath(word)){
        srvr.duplicate_set.insert("upload_path");
        srvr.duplicate_vec.push_back("upload_path");

    }
    else if(srvr.get_error_page(word)){}
}


void Server::get_file(std::string file){
    BracketsCheck(file);
    std::ifstream ss;
    ss.open(file.c_str());
     if (!ss.is_open())
        throw std::invalid_argument("can't open file");
    std::string line;
    while (std::getline(ss, line)){
        if(line.find("server") != std::string::npos){
            removeSpaces(line);
            if(line != "server{")
                throw std::invalid_argument("server: invalid declaration");

            Server srvr;
            while(std::getline(ss,line) && line != "}"){
                if (line.find("location") != std::string::npos){
                    for (size_t i = 0; i < line.length(); i++){
                        if (line[i] == '}')
                            throw std::invalid_argument("location :Brackets!!!!");
                    }
                    Location location;
                    if (location.get_location_Name(line)){
                        srvr.duplicate_set.insert(location.NAME);
                        srvr.duplicate_vec.push_back(location.NAME);
                    }
                    while (std::getline(ss, line) && line.find("}") == std::string::npos){
                        if (line.find(';') == std::string::npos){
                            removeSpaces(line);
                            if (!line.empty())
                                throw std::invalid_argument("invalid input outside the server context");
                        }
                        else{
                            location.fill_location(line, location);
                            if (location.vec_dup.size() != location.set_dup.size())
                                throw std::invalid_argument("location: duplicate directive");
                            else if (!location.upload.size())
                                location.upload.push_back("off");
                        }
                    }

                    srvr.locations.push_back(location);
                    if(location.vec_dup.size() != location.set_dup.size())
                        throw std::invalid_argument("location: duplicate");
                }
                else if (line.find(';') == std::string::npos){
                    removeSpaces(line);
                    if (!line.empty())
                        throw std::invalid_argument("invalid input outside the server context");
                }
                else
                    srvr.fill_server(line, srvr);
            }
            if (srvr.listen.size() < 1 || srvr.host == "" || srvr.locations.size() < 1)
                throw std::invalid_argument("Serever: the server at least must have a port, host and location");
             if (srvr.duplicate_set.size() != srvr.duplicate_vec.size())
                throw std::invalid_argument("server: duplicate directive!");
            else{
                if (!srvr.rootFlag){
                    for(size_t i = 0; i < srvr.locations.size(); i++){
                        if (!srvr.locations[i].rootFlag && !srvr.locations[i].aliasFlag){
                            throw std::invalid_argument("location must have a root");
                        }
                    }
                }
                this->servr.push_back(srvr);
            }
        }
        else{
            removeSpaces(line);
            if (!line.empty())
                throw std::invalid_argument("invalid input outside the server context");
        }
    }
}