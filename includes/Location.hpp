#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <iostream>
#include <vector>
#include <set>

class Location{
    public:
        bool        cgi;
        bool        autoindex;
        bool        rootFlag;
        bool        aliasFlag;
        std::string NAME;
        std::string root;
        std::string alias;
        std::string cgi_path;
        
        std::vector<std::string>    allowed_methods;
        std::vector<std::string>    index;
        std::vector<std::string>    redirections;
        std::vector<std::string>    upload;
        std::vector<std::string>    error_pages;
        std::vector<std::string>    vec_dup;
        std::set<std::string>       set_dup;

        Location();
        void    fill_location(std::string, Location&);
        int     get_location_Name(std::string);
        int     get_root(std::vector<std::string>);
        int     get_autoindex(std::vector<std::string>);
        int     get_methods(std::vector<std::string>, Location&);
        int     get_index(std::vector<std::string> word);
        int     get_upload(std::vector<std::string> word);
        int     get_cgi(std::vector<std::string> word);
        int     get_redirect(std::vector<std::string> word);
        int     get_alias(std::vector<std::string> word);
        int     get_error_page(std::vector<std::string> word);
        int     getCgiPath(std::vector<std::string> word);
        void    checkerDuplicate(Location& location, std::string directive);
};




#endif