NAME = ft_ping
CC = gcc
CFLAGS = -Wall -Werror -Wextra -fsanitize=address -g3
SRC = main.c socket.c
OBJ = $(SRC:.c=.o)
INC = inc

all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $^

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
