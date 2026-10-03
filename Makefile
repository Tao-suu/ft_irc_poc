# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: picheval <picheval@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/21 11:48:55 by picheval          #+#    #+#              #
#    Updated: 2026/10/03 15:04:54 by picheval         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC 					= c++
CFLAGS				= -Wall -Wextra -Werror -std=c++98 -g3

NAME				= ircserv
BOTS_NAME			= ircbots

SRCS_FOLDER			= ./srcs/
OBJS_FOLDER			= ./objs/
HEADERS_FOLDER		= ./headers/
INC_FOLDER			= ./includes/
DEPS_FOLDER			= ./deps/
PARSING_FOLDER		= parsing/
INCLUDES			= -I$(INC_FOLDER) \
					  -I$(HEADERS_FOLDER) \
					  -I$(HEADERS_FOLDER)$(PARSING_FOLDER)

BOTS_SRCS_FOLDER	= ./srcs_bots/
BOTS_OBJS_FOLDER	= ./objs_bots/
BOTS_HEADERS_FOLDER	= ./headers_bots/
BOTS_DEPS_FOLDER	= ./deps_bots/
BOTS_INCLUDES		= -I$(INC_FOLDER) \
					  -I$(BOTS_HEADERS_FOLDER)

SRCS_RAW			= main.cpp \
					  $(PARSING_FOLDER)Validator.cpp \
					  $(PARSING_FOLDER)CommandValidator.cpp \
					  channel.cpp \
					  client.cpp \
					  Server.cpp \
					  MessageIn.cpp \
					  Error.cpp \
					  MessageOut.cpp \
					  utils.cpp
SRCS				= $(addprefix $(SRCS_FOLDER), $(SRCS_RAW))
OBJS_RAW			= $(SRCS_RAW:.cpp=.o)
OBJS				= $(addprefix $(OBJS_FOLDER), $(OBJS_RAW))
DEPS_RAW			= $(SRCS_RAW:.cpp=.d)
DEPS				= $(addprefix $(DEPS_FOLDER), $(DEPS_RAW))

BOTS_SRCS_RAW		= main.cpp \
					  serverTools.cpp \
					  tools.cpp \
					  Bot.cpp
BOTS_SRCS			= $(addprefix $(BOTS_SRCS_FOLDER), $(BOTS_SRCS_RAW))
BOTS_OBJS_RAW		= $(BOTS_SRCS_RAW:.cpp=.o)
BOTS_OBJS			= $(addprefix $(BOTS_OBJS_FOLDER), $(BOTS_OBJS_RAW))
BOTS_DEPS_RAW		= $(BOTS_SRCS_RAW:.cpp=.d)
BOTS_DEPS			= $(addprefix $(BOTS_DEPS_FOLDER), $(BOTS_DEPS_RAW))

C_NONE				= \033[0m
C_RED				= \033[1;38;5;196;49m
C_BLUE				= \033[1;38;5;33;49m

SYMBOL				= $(C_RED)==> $(C_NONE)


all: $(NAME)

re: fclean all

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(LIBS_FLAGS) $^ -o $@
	@echo "$(SYMBOL)$(C_BLUE)$(NAME) compiled !$(C_NONE)"

$(BOTS_NAME): $(BOTS_OBJS)
	@$(CC) $(CFLAGS) $(LIBS_FLAGS) $^ -o $@
	@echo "$(SYMBOL)$(C_BLUE)$(BOTS_NAME) compiled !$(C_NONE)"

-include $(DEPS)
$(OBJS_FOLDER)%.o: $(SRCS_FOLDER)%.cpp
	@mkdir -p $(OBJS_FOLDER)$(PARSING_FOLDER)
	@mkdir -p $(DEPS_FOLDER)$(PARSING_FOLDER)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -MMD -MP -MF $(DEPS_FOLDER)$*.d

-include $(BOTS_DEPS)
$(BOTS_OBJS_FOLDER)%.o: $(BOTS_SRCS_FOLDER)%.cpp
	@mkdir -p $(BOTS_OBJS_FOLDER)
	@mkdir -p $(BOTS_DEPS_FOLDER)
	@$(CC) $(CFLAGS) $(BOTS_INCLUDES) -c $< -o $@ -MMD -MP -MF $(BOTS_DEPS_FOLDER)$*.d

clean:
	@rm -rf $(OBJS_FOLDER) $(DEPS_FOLDER) $(BOTS_OBJS_FOLDER) $(BOTS_DEPS_FOLDER)
	@echo "$(SYMBOL)$(C_BLUE)$(NAME) clean done !$(C_NONE)"

fclean: clean
	@rm -f $(NAME) $(BOTS_NAME)
	@echo "$(SYMBOL)$(C_BLUE)$(NAME) fclean done !$(C_NONE)"

.PHONY: all re clean fclean

# SRC		= main.cpp \
# 			Server.cpp \
# 			client.cpp
# OBJ		= $(SRC:.cpp=.o)
# DEP		= $(SRC:.cpp=.d)

# NAME	= ft_irc

# CC		= c++
# CF		= -Wall -Werror -Wextra -std=c++98


# all: $(NAME)

# $(NAME):	$(OBJ)
# 	$(CC) $(CF) $(OBJ) -o $(NAME)

# -include $(DEP)
# %.o:	%.cpp
# 	$(CC) $(CF) -c $< -o $@ -MMD -MP -MF $*.d

# clean:
# 	rm -f $(OBJ) $(DEP)

# fclean:	clean
# 	rm -f $(NAME)
# re:	clean all

# .PHONY: all clean fclean re