#include "../includes/Client.hpp"
#include <ctime>
#include <cstdlib>
#include <fstream>



std::string Client::generateRandomString(int length) {
    std::srand(std::time(0));
    const char characters[] = "abcdefghijklmnopqrstuvwxyz";
    int numCharacters = sizeof(characters) - 1;
    std::string randomString;

    for (int i = 0; i < length; ++i)
    {
        int randomIndex = std::rand() % numCharacters;
        randomString += characters[randomIndex];
    }
    return randomString;
}

std::string FileType(std::string path){
    if (path == "text/html ")
        return (".html");
    else if (path == "application/x-www-form-urlencoded")                         
        return (".txt");
    else if (path == "text/css")
        return (".css");
    else if (path == "application/x-httpd-php")
        return (".txt");
    else if (path == "text/xml")
        return (".xml");
    else if (path == "image/gif ")
        return (".gif");
    else if (path == "image/jpeg")
        return (".jpeg");
    else if (path == "application/javascript")
        return (".js");
    else if (path == "application/atom+xml")
        return (".atom");
    else if (path == "application/rss+xml")
        return ("rss.");
    else if (path == "text/mathml")
        return (".mml");
    else if (path == "text/plain")
        return (".txt");
    else if (path == "text/vnd.sun.j2me.app-descriptor")
        return (".jad");
    else if (path == "text/vnd.wap.wml")
        return (".wml");
    else if (path == "text/x-component")
        return (".htc");
    else if (path == "image/avif")
        return (".avif");
    else if (path == "image/png")
        return (".png");
    else if (path == "image/svg+xml")
        return (".svg");
    else if (path == "image/tiff")
        return (".tif");
    else if (path == "image/vnd.wap.wbmp")
        return (".wbmp");
    else if (path == "image/webp")
        return (".webp");
    else if (path == "image/x-icon")
        return (".ico");
    else if (path == "image/x-jng")
        return (".jng");
    else if (path == "image/x-ms-bmp")
        return (".bmp");
    else if (path == "font/woff")                                        
        return (".woff");
    else if (path == "font/woff2")                                       
        return (".woff2");
    else if (path == "application/java-archive")                         
        return (".jar war ear");
    else if (path == "application/json")                                 
        return (".json");
    else if (path == "application/mac-binhex40")                         
        return (".hqx");
    else if (path == "application/msword")                               
        return (".doc");
    else if (path == "application/pdf")                                  
        return (".pdf");
    else if (path == "application/postscript")                           
        return (".ps eps ai");
    else if (path == "application/rtf")                                  
        return (".rtf");
    else if (path == "application/vnd.apple.mpegurl")                    
        return (".m3u8");
    else if (path == "application/vnd.google-earth.kml+xml")             
        return (".kml");
    else if (path == "application/vnd.google-earth.kmz")                 
        return (".kmz");
    else if (path == "application/vnd.ms-excel")                         
        return (".xls");
    else if (path == "application/vnd.ms-fontobject")                    
        return (".eot");
    else if (path == "application/vnd.ms-powerpoint")                    
        return (".ppt");
    else if (path == "application/vnd.oasis.opendocument.graphics")      
        return (".odg");
    else if (path == "application/vnd.oasis.opendocument.presentation")  
        return (".odp");
    else if (path == "application/vnd.oasis.opendocument.spreadsheet")   
        return (".ods");
    else if (path == "application/vnd.oasis.opendocument.text")          
        return (".odt");
    else if (path == "application/vnd.openxmlformats-officedocument.presentationml.presentation")   
        return (".pptx");
    else if (path == "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")           
        return (".xlsx");
    else if (path == "application/vnd.openxmlformats-officedocument.wordprocessingml.document")     
        return (".docx");
    else if (path == "application/vnd.wap.wmlc")                         
        return (".wmlc");
    else if (path == "application/wasm")                                 
        return (".wasm");
    else if (path == "application/x-7z-compressed")                      
        return (".7z");
    else if (path == "application/x-cocoa")                              
        return (".cco");
    else if (path == "application/x-java-archive-diff")                  
        return (".jardiff");
    else if (path == "application/x-java-jnlp-file")                     
        return (".jnlp");
    else if (path == "application/x-makeself")                           
        return (".run");
    else if (path == "application/x-perl")                               
        return (".pl");
    else if (path == "application/x-pilot")                              
        return (".prc");
    else if (path == "application/x-rar-compressed")                     
        return (".rar");
    else if (path == "application/x-redhat-package-manager")             
        return (".rpm");
    else if (path == "application/x-sea")                                
        return (".sea");
    else if (path == "application/x-shockwave-flash")                    
        return (".swf");
    else if (path == "application/x-stuffit")                            
        return (".sit");
    else if (path == "application/x-tcl")                                
        return (".tcl");
    else if (path == "application/x-x509-ca-cert")                       
        return (".der");
    else if (path == "application/x-xpinstall")                          
        return (".xpi");
    else if (path == "application/xhtml+xml")                            
        return (".xhtml");
    else if (path == "application/xspf+xml")                             
        return (".xspf");
    else if (path == "application/zip")                                  
        return (".zip");
    else if (path == "application/octet-stream")                         
        return (".bin");
    else if (path == "audio/midi")                                       
        return (".mid");
    else if (path == "audio/mpeg")                                       
        return (".mp3");
    else if (path == "audio/ogg")                                        
        return (".ogg");
    else if (path == "audio/x-m4a")                                      
        return (".m4a");
    else if (path == "audio/x-realaudio")                                
        return (".ra");
    else if (path == "video/3gpp")                                       
        return (".3gpp");
    else if (path == "video/mp2t")                                       
        return (".ts");
    else if (path == "video/mp4")                                        
        return (".mp4");
    else if (path == "video/mpeg")                                       
        return (".mpeg");
    else if (path == "video/quicktime")                                  
        return (".mov");
    else if (path == "video/webm")                                       
        return (".webm");
    else if (path == "video/x-flv")                                      
        return (".flv");
    else if (path == "video/x-m4v")                                      
        return (".m4v");
    else if (path == "video/x-mng")                                      
        return (".mng");
    else if (path == "video/x-ms-asf")                                   
        return (".asx asf");
    else if (path == "video/x-ms-wmv")                                   
        return (".wmv");
    else if (path == "video/x-msvideo")                                  
        return (".avi");
    return "404";
}

std::string Client::createRandomStr(Client&client, Server& server){
    for(size_t i = 0; i < client.request.data["Content-Length"].size(); i++)
    {
        if (!isdigit(client.request.data["Content-Length"][i])){
            return "400";
        }
    }
    std::stringstream str(client.request.data["Content-Length"]);
    str >> client.content_length;
    std::string type = client.request.data["Content-Type"];
    if (server.uploadPath.empty())
        server.uploadPath = "uploads";
    DIR* dir = opendir(server.uploadPath.c_str());
    if(!dir)
        server.uploadPath = ".";
    else
        closedir(dir);
    std::string fileExtension = FileType(type);
    if (fileExtension == "404")
    {
        return "404";
    }
    if (fileExtension == "404")
        return "404";
    
    std::string randomm = client.generateRandomString(4);
    client.fileName = randomm.append(fileExtension);
    std::string tmp = server.uploadPath + "/" + client.fileName;
    return tmp;
}

int openFile(std::ofstream& file, Client& client, Server& server){
    client.fileName = client.createRandomStr(client, server);
    if(client.fileName == "404")
        return 404;
    else if (client.fileName == "400")
        return 400;
    file.open(client.fileName.c_str(), std::ios::app);
    if (!file.is_open()){
        return 404;
    }
    return 1;
}


std::string Client::getUploadSuccesPage()
{
    std::string page = "<!DOCTYPE html>\n"
                       "<html lang=\"en\">\n"
                       "<head>\n"
                       "<meta charset=\"UTF-8\">\n"
                       "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                       "<style>\n"
                       "body {\n"
                       "    background-color: #202c3c;\n"
                       "    display: flex;\n"
                       "    justify-content: center;\n"
                       "    align-items: center;\n"
                       "    height: 100vh;\n"
                       "    margin: 0;\n"
                       "}\n"
                       ".success-animation {\n"
                       "    text-align: center;\n"
                       "}\n"
                       "h1 {\n"
                       "    color: #4bb71b;\n"
                       "    font-family: 'Open Sans', sans-serif;\n"
                       "    font-weight: 300;\n"
                       "    font-size: 40px;\n"
                       "    line-height: 32px;\n"
                       "    margin-top: 50px;\n"
                       "    text-align: center;\n"
                       "    animation: opacity 0.6s ease-in-out;\n"
                       "}\n"
                       ".checkmark {\n"
                       "    width: 200px;\n"
                       "    height: 200px;\n"
                       "    border-radius: 50%;\n"
                       "    display: inline-block;\n"
                       "    stroke-width: 2;\n"
                       "    stroke: #4bb71b;\n"
                       "    stroke-miterlimit: 5;\n"
                       "    box-shadow: inset 0px 0px 0px #4bb71b;\n"
                       "    animation: fill .4s ease-in-out .4s forwards, scale .3s ease-in-out .9s both;\n"
                       "    margin: 0 auto;\n"
                       "}\n"
                       ".checkmark__circle {\n"
                       "    stroke-dasharray: 166;\n"
                       "    stroke-dashoffset: 166;\n"
                       "    stroke-width: 1;\n"
                       "    stroke-miterlimit: 10;\n"
                       "    stroke: #4bb71b;\n"
                       "    animation: stroke 0.6s cubic-bezier(0.65, 0, 0.45, 1) forwards;\n"
                       "}\n"
                       ".checkmark__check {\n"
                       "    transform-origin: 50% 50%;\n"
                       "    stroke-dasharray: 48;\n"
                       "    stroke-dashoffset: 48;\n"
                       "    animation: stroke 0.3s cubic-bezier(0.65, 0, 0.45, 1) 0.8s forwards;\n"
                       "}\n"
                       "@keyframes stroke {\n"
                       "    100% {\n"
                       "        stroke-dashoffset: 0;\n"
                       "    }\n"
                       "}\n"
                       "@keyframes scale {\n"
                       "    0%, 100% {\n"
                       "        transform: none;\n"
                       "    }\n"
                       "    50% {\n"
                       "        transform: scale3d(1.1, 1.1, 1);\n"
                       "    }\n"
                       "}\n"
                       "@keyframes fill {\n"
                       "    100% {\n"
                       "        box-shadow: inset 0px 0px 0px 20px #4bb71b;\n"
                       "    }\n"
                       "}\n"
                       "@keyframes opacity {\n"
                       "    0% {\n"
                       "        opacity: 0;\n"
                       "    }\n"
                       "    100% {\n"
                       "        opacity: 1;\n"
                       "    }\n"
                       "}\n"
                       "</style>\n"
                       "</head>\n"
                       "<body>\n"
                       "<div class=\"success-animation\">\n"
                       "<svg class=\"checkmark\" xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 52 52\"><circle class=\"checkmark__circle\" cx=\"26\" cy=\"26\" r=\"25\" fill=\"none\" /><path class=\"checkmark__check\" fill=\"none\" d=\"M14.1 27.2l7.1 7.2 16.7-16.8\" /></svg>\n"
                       "<h1>UPLOADED SUCCESSFULY</h1>\n"
                       "</div>\n"
                       "</body>\n"
                       "</html>";

    return page;
}

ssize_t Client::getFileSize(Client& client){
    struct stat st;
    stat(client.fileName.c_str(), &st);
    return (st.st_size);
}

void Client::postContentLength(Client& client, Server& server){
    static std::ofstream file;
    if (client.fileCreated == false){
        int x = openFile(file, client, server);
        if (x == 404)
        {
            client.response.throwErrorPage(server, -1, 415);
            client.postCompleted = true;
            return ;      
        }
        else if (x == 400)
        {
            client.response.throwErrorPage(server, -1, 400);
            client.postCompleted = true;
            return ;      
        }
        client.count_bodyLen = client.request.body.size();
        if (client.count_bodyLen > client.content_length)
            file.write(client.request.body.c_str(), client.content_length);
        else
            file.write(client.request.body.c_str(), client.request.body.size());
        client.fileCreated = true;
    }
    else{
        client.count_bodyLen += client.readBytes;
        if (client.count_bodyLen > client.content_length){
            file.write(client.buffer, ((client.content_length + client.readBytes) - client.count_bodyLen));
        }
        else
            file.write(client.buffer, client.readBytes);
    }
    if (client.count_bodyLen >= client.content_length)
    {
        client.request.body.clear();
        file.close();
        client.postCompleted = true;
        if (server.max_body <= getFileSize(client)){
            remove(client.fileName.c_str());
            client.response.throwErrorPage(server, -1, 413);
            return;
        }
        if (server.locations[client.response.getLocationIndex()].cgi 
            && (client.ifExtensionCgi(client.realPath))){
            client.cgi.serveCgi(server, client.response.getLocationIndex(), client, client.realPath);
            return ;
        }
        if(client.CgiCopmpleted == false || postCompleted == true){
            std::string response = "HTTP/1.1 201 Created\r\n";
            response += "Connection: close\r\n\r\n";
            response += client.getUploadSuccesPage();   
            client.response.setResponse(response);
            client.postCompleted = true;
        }
    }
}

void getDecimalSize(Client& client, std::string buff){
    std::string hexaStr = buff.substr(0, \
        buff.find("\r\n"));
    client.strChunk.erase(0,  buff.find("\r\n")+2);
    client.counterChunk =  client.strChunk.size();
    std::stringstream stream;
    stream << std::hex << hexaStr;
    stream >> client.convertedHexaValue;
}

void Client::postChuncked(Client &client, Server& server){
    static std::ofstream file;
    if (!client.firstReadChunk){
        client.strChunk = client.request.body;
        getDecimalSize(client, client.request.body);
        client.firstReadChunk = true;
        if (openFile(file, client, server) == 404){
            client.response.throwErrorPage(server, client.response.getLocationIndex(), 404);
            client.postCompleted = true;
            return ;
        }
    }
    else{
        client.strChunk.append(client.buffer, client.readBytes);
        client.counterChunk += client.readBytes;
    }
    if(client.counterChunk > client.convertedHexaValue 
            && client.strChunk.find("\r\n", client.convertedHexaValue + 2) \
            != std::string::npos){
        file.write(client.strChunk.c_str(), client.convertedHexaValue);
        client.strChunk.erase(0, client.convertedHexaValue+2);
        client.counterChunk -= (client.convertedHexaValue + 2);
        getDecimalSize(client, client.strChunk);
    }
    if(!client.convertedHexaValue){
        file.close();
        client.postCompleted = true;
        if (server.max_body <= getFileSize(client)){
            remove(client.fileName.c_str());
            client.response.throwErrorPage(server, -1, 413);
            return;
        }
        if (server.locations[client.response.getLocationIndex()].cgi 
            && client.ifExtensionCgi(client.realPath)){
            client.cgi.serveCgi(server, client.response.getLocationIndex(), client, client.realPath);
            return ;
        }
        if(client.CgiCopmpleted == false)
        {
            std::string response = "HTTP/1.1 201 Created\r\n";
            response += "Connection: close\r\n\r\n";
            response += client.getUploadSuccesPage();   
            client.response.setResponse(response);
            client.postCompleted = true;
        }
    }
}