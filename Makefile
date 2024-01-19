NAME			=	ircserv

CXX				=	c++
RM				=	rm -rf
CXXFLAGS		=	-Wall -Wextra -Werror -std=c++98

HEADER			=	./src

# ---------------------------------------------------------------------

SRCS_DIR		=	./src
SRCS_FILES		=	main.cpp \
					Channel.cpp \
					Client.cpp \
					Server.cpp \
					checkCmd.cpp \
					Utils.cpp
SRCS			=	$(addprefix $(SRCS_DIR)/, $(SRCS_FILES))

CMD_DIR			=	./src/command/cmd
CMD_FILES		=	list.cpp \
					nick.cpp \
					pass.cpp \
					pong.cpp \
					quit.cpp \
					user.cpp \
					userhost.cpp \
					whois.cpp
CMD				=	$(addprefix $(CMD_DIR)/, $(CMD_FILES))

CHANNEL_DIR		=	./src/command/inChannel
CHANNEL_FILES	=	join.cpp \
					kick.cpp \
					part.cpp \
					privMsg.cpp \
					topic.cpp
CHANNEL			=	$(addprefix $(CHANNEL_DIR)/, $(CHANNEL_FILES))

# ---------------------------------------------------------------------

OBJS_DIR		=	./objs
OBJS			=	$(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o) \
					$(CMD:$(CMD_DIR)/%.cpp=$(OBJS_DIR)/%.o) \
					$(CHANNEL:$(CHANNEL_DIR)/%.cpp=$(OBJS_DIR)/%.o)

# ---------------------------------------------------------------------

all	:				$(NAME)

$(OBJS_DIR):
					mkdir $@

$(OBJS_DIR)/%.o	:	$(SRCS_DIR)/%.cpp
					$(CXX) $(CXXFLAGS) -I $(HEADER) -c $< -o $@

$(OBJS_DIR)/%.o	:	$(CMD_DIR)/%.cpp
					$(CXX) $(CXXFLAGS) -I $(HEADER) -c $< -o $@

$(OBJS_DIR)/%.o	:	$(CHANNEL_DIR)/%.cpp
					$(CXX) $(CXXFLAGS) -I $(HEADER) -c $< -o $@

# ---------------------------------------------------------------------

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
