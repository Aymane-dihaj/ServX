#include "../includes/webserv.hpp"

Location::Location(){

    this->root = "";
    this->autoindex = false;
    this->aliasFlag = false;
    this->rootFlag = false;
}


int Location::get_location_Name(std::string line){
        if (line[line.length() -1] != '{')
            throw std::invalid_argument("locaton: can't find '{'!");
        line.erase(line.find('{'));
        std::vector<std::string> word = split_line(line, " ");
        if (word.size() != 2)
            throw std::invalid_argument("location: invalid location name!!");
        if (word[1][0] != '/')
            throw std::invalid_argument("location: invalid path");
        this->NAME = word[1];
        return 1;
}

int Location::get_root(std::vector<std::string> word){
    if (word[0] == "root"){

        if(word.size() != 2)
            throw std::invalid_argument("location: root: invalid input");
        this->root = word[1];
        return 1;
    }
    return 0;
}

int Location::get_autoindex(std::vector<std::string> word){
    if (word[0] == "auto_index"){
        if (word.size() != 2)
            throw std::invalid_argument("location: autoindex: invalid input");
        if (word[1] != "on" && word[1] != "off")
            throw std::invalid_argument("location: autoindex: invalid input");
        else if (word[1] == "on")
            this->autoindex = true;
        else if (word[1] == "off")
            this->autoindex = false;
        return 1;
    }
    return 0;
}

int Location::get_methods(std::vector<std::string> word, Location& location)
{
    (void)location;
    if (word[0] == "allowed_methods"){
        for (size_t i = 1; i < word.size(); i++)
            if(word[i] != "POST" && word[i] != "DELETE" && word[i] != "GET")
                throw std::invalid_argument("allowed_methods: invalid method");

        for (size_t i = 1; i < word.size(); i++){
            this->allowed_methods.push_back(word[i]);
        }
        return 1;
    }
    return 0;
}

int Location::get_index(std::vector<std::string> word){
    if (word[0] == "index"){

        if(word.size() < 2)
            throw std::invalid_argument("location: index: invalid input");
        for (size_t i = 1; i < word.size(); i++){
            this->index.push_back(word[i]);
        }
        return 1;
    }
    return 0;
}

int Location::get_upload(std::vector<std::string> word){
    if(word[0] == "upload"){
        if(word[1] == "on" && word.size() == 3){
            this->upload.push_back(word[1]);
            this->upload.push_back(word[2]);
        }
        else if (word[1] == "off" && word.size() == 2)
            this->upload.push_back(word[1]);
        else
            throw std::invalid_argument("location: upload: invalid input");
        return 1;
    }
    return 0;
}

int Location::get_cgi(std::vector<std::string> word){
    if(word[0] == "cgi"){
        if (word.size() != 2)
            throw std::invalid_argument("location: cgi: invalid input");
        if (word[1] != "on" && word[1] != "off")
            throw std::invalid_argument("location: cgi: invalid input");
        else if (word[1] == "on")
            this->cgi = true;
        else if (word[1] == "off")
            this->cgi = false;
        return 1;
    }
    return 0;
}

int Location::get_redirect(std::vector<std::string> word)
{
    if ("return" == word[0]){
        if (word.size() != 3)
            throw std::invalid_argument("redirect: missing code or target..!");
        if (word[1].length() != 3)
            throw std::invalid_argument("redirert: invalid code");
        for (size_t i = 1; i < word.size(); i++)
            this->redirections.push_back(word[i]);
        return 1;
    }
    return 0;
}

int Location::get_alias(std::vector<std::string> word){
     if (word[0] == "alias"){
        if(word.size() != 2)
            throw std::invalid_argument("location: alias: invalid input");
        this->alias = word[1];
        return 1;
    }
    return 0;
}

int Location::get_error_page(std::vector<std::string> word){
    if ("error_page" == word[0]){
        if (word.size() != 3)
            throw std::invalid_argument("location: error_pages: missing code or target..!");
        if (word[1].length() != 3 || (std::stoi(word[1]) < 400 || std::stoi(word[1]) > 599))
            throw std::invalid_argument("location: error_pages: invalid code");
        for (size_t i = 1; i < word.size(); i++)
            this->error_pages.push_back(word[i]);
        return 1;
    }
    return 0;
}

int Location::getCgiPath(std::vector<std::string> word){
    if(word[0] == "cgi_path"){
        if (word.size() != 2)
            throw std::invalid_argument("location: cgi_path: invalid input");
        this->cgi_path = word[1];
        return 1;
    }
    return 0;
}

void Location::checkerDuplicate(Location& location, std::string directive){
    location.vec_dup.push_back(directive);
    location.set_dup.insert(directive);
}

void Location::fill_location(std::string line, Location& location){
    if (line.find(";") == std::string::npos)
        throw std::invalid_argument("location: invalid form");
    line.erase(line.find(";"));
    std::vector<std::string> word = split_line(line, " ");
    if (word[0] != "root" && word[0] != "auto_index" && word[0] != "allowed_methods"\
            && word[0] != "index" && word[0] != "upload" && word[0] != "error_page"\
            && word[0] != "cgi" && word[0] != "return" && word[0] != "alias"\
            && word[0] != "cgi_path")
        throw std::invalid_argument("location: invalid directive !!!!!");
    
    else if (word.size() < 2)
        throw std::invalid_argument("location: incorect input");
    else if (location.get_root(word)){
        checkerDuplicate(location, "root");
        location.rootFlag = true;
    }
    else if(location.get_autoindex(word))
        checkerDuplicate(location, "autoindex");
    else if(location.get_index(word))
        checkerDuplicate(location, "index");
    else if (location.get_methods(word, location))
        checkerDuplicate(location, "allowed_methods");
    else if (location.get_upload(word))
        checkerDuplicate(location, "uploads");
    else if (location.get_cgi(word))
        checkerDuplicate(location, "cgi");
    else if(location.get_redirect(word))
        checkerDuplicate(location, "return");
    else if(location.get_alias(word)){
        checkerDuplicate(location, "alias");
        location.aliasFlag = true;
    }
    else if(location.getCgiPath(word))
        checkerDuplicate(location, "cgi_path");
    else if(location.get_error_page(word)){}
}