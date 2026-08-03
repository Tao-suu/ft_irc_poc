SRC		= main.cpp
OBJ		= $(SRC:.cpp=.o)
DEP		= $(SRC:.cpp=.d)

NAME	= ft_irc

CC		= c++
CF		= -Wall -Werror -Wextra -std=c++98


all: $(NAME)

$(NAME):	$(OBJ)
	$(CC) $(CF) $(OBJ) -o $(NAME)

-include $(DEP)
%.o:	%.cpp
	$(CC) $(CF) -c $< -o $@ -MMD -MP -MF $*.d

clean:
	rm -f $(OBJ) $(DEP)

fclean:	clean
	rm -f $(NAME)
re:	clean all

.PHONY: all clean fclean re