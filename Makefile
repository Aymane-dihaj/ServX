NAME = webserv

SRCS = Server/main.cpp \
		Server/ServerManager.cpp \
		Server/Client.cpp \
		config/Location.cpp \
		config/Config.cpp \
		Request/Request.cpp \
		Response/Response.cpp \
		Response/GET.cpp \
		Response/utils.cpp \
		Response/DELETE.cpp \
		Post/Post.cpp \
		CGI/Cgi.cpp \


CPPFLAGS = -std=c++98 -Wall -Wextra -Werror
CXX = c++
OBJ = $(SRCS:.cpp=.o)
all : $(NAME)

$(NAME) : $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)
	rm -f uploads/*

re : fclean all 