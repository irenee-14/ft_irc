BLACK		= 	"\033[30m"
GRAY		= 	"\033[30m"
RED			=	"\033[31m"
GREEN		=	"\033[32m"
YELLOW		=	"\033[33m"
BLUE		=	"\033[34m"
PURPLE		=	"\033[35m"
CYAN		=	"\033[36m"
WHITE		=	"\033[37m"
BOLD		=	"\033[01m"
ITALIC		=	"\033[3m"
EOC			=	"\033[0m"
LINE_DEL	=	"\x1b[1A\x1b[M"

# ---------------------------------------------------------------------

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


MODE_DIR		=	./src/command/channelMode
MODE_FILES		=	mode.cpp \
					processMode.cpp
MODE			=	$(addprefix $(MODE_DIR)/, $(MODE_FILES))

CHANNEL_DIR		=	./src/command/channelOper
CHANNEL_FILES	=	invite.cpp \
					join.cpp \
					kick.cpp \
					list.cpp \
					part.cpp \
					privMsg.cpp \
					topic.cpp
CHANNEL			=	$(addprefix $(CHANNEL_DIR)/, $(CHANNEL_FILES))


CONNECT_DIR		=	./src/command/connectionMsg
CONNECT_FILES	=	nick.cpp \
					pass.cpp \
					pong.cpp \
					quit.cpp \
					user.cpp
CONNECT			=	$(addprefix $(CONNECT_DIR)/, $(CONNECT_FILES))

OPTION_DIR		=	./src/command/optionalMsg
OPTION_FILES	=	motd.cpp \
					userhost.cpp \
					whois.cpp \
					bot.cpp
OPTION			=	$(addprefix $(OPTION_DIR)/, $(OPTION_FILES))


# ---------------------------------------------------------------------

OBJS_DIR		=	./objs
OBJS			=	$(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o) \
					$(MODE:$(MODE_DIR)/%.cpp=$(OBJS_DIR)/%.o) \
					$(CHANNEL:$(CHANNEL_DIR)/%.cpp=$(OBJS_DIR)/%.o) \
					$(CONNECT:$(CONNECT_DIR)/%.cpp=$(OBJS_DIR)/%.o) \
					$(OPTION:$(OPTION_DIR)/%.cpp=$(OBJS_DIR)/%.o)

# ---------------------------------------------------------------------

all	:				$(NAME)

$(OBJS_DIR):
					@echo "\n"
					@mkdir $@

$(OBJS_DIR)/%.o	:	$(SRCS_DIR)/%.cpp
					@echo $(GRAY) "ircserv Compiling... " $< $(EOC) $(LINE_DEL)
					@$(CXX) $(CXXFLAGS) -I $(HEADER) -c $< -o $@

$(OBJS_DIR)/%.o	:	$(MODE_DIR)/%.cpp
					@echo $(GRAY) "ircserv Compiling... " $< $(EOC) $(LINE_DEL)
					@$(CXX) $(CXXFLAGS) -I $(HEADER) -c $< -o $@

$(OBJS_DIR)/%.o	:	$(CHANNEL_DIR)/%.cpp
					@echo $(GRAY) "ircserv Compiling... " $< $(EOC) $(LINE_DEL)
					@$(CXX) $(CXXFLAGS) -I $(HEADER) -c $< -o $@

$(OBJS_DIR)/%.o	:	$(CONNECT_DIR)/%.cpp
					@echo $(GRAY) "ircserv Compiling... " $< $(EOC) $(LINE_DEL)
					@$(CXX) $(CXXFLAGS) -I $(HEADER) -c $< -o $@


$(OBJS_DIR)/%.o	:	$(OPTION_DIR)/%.cpp
					@echo $(GRAY) "ircserv Compiling... " $< $(EOC) $(LINE_DEL)
					@$(CXX) $(CXXFLAGS) -I $(HEADER) -c $< -o $@

# ---------------------------------------------------------------------

$(NAME)	:			$(OBJS_DIR) $(OBJS)
					@$(CXX) $(CXXFLAGS) -I $(HEADER) -o $(NAME) $(OBJS)
					@echo $(BOLD) $(BLUE) \
					"\n========================================================\n" $(EOC)
					@echo $(BOLD) $(ITALIC) $(BLUE) \
					"              << ircserv is made! >>\n\n" $(EOC)
clean	:
					@$(RM) $(OBJS_DIR)

fclean	: 			clean
					@$(RM) $(NAME)
re	:
					@make fclean
					@make all

.PHONY:		all clean fclean re
