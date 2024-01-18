NAME			=	ircserv

CXX				=	c++
RM				=	rm -rf
CXXFLAGS		=	-Wall -Wextra -Werror -std=c++98

HEADER			=	./src

SRCS_DIR		=	./src
SRCS_FILES		=	main.cpp \
					Channel.cpp \
					Client.cpp \
					Command.cpp \
					Server.cpp \
					Utils.cpp
SRCS			=	$(addprefix $(SRCS_DIR)/, $(SRCS_FILES))

OBJS			=	$(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)
OBJS_DIR		=	./objs

all	:				$(NAME)

$(OBJS_DIR):
					mkdir $@

$(OBJS_DIR)/%.o	:	$(SRCS_DIR)/%.cpp
					$(CXX) $(CXXFLAGS) -I $(HEADER) -c $< -o $@

$(NAME)	:			$(OBJS_DIR) $(OBJS)
					$(CXX) $(CXXFLAGS) -I $(HEADER) -o $(NAME) $(OBJS)

clean	:
					$(RM) $(OBJS_DIR)

fclean	: 			clean
					$(RM) $(NAME)
re	:
					make fclean
					make all

.PHONY:		all clean fclean re
