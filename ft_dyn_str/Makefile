.PHONY: all clean fclean re objects objects_bonus bonus norm

NAME=ft_dyn_str.a
FILES=ft_dyn_str.c
OBJECT=$(FILES:.c=.o)
CFLAGS=-Wall -Wextra -Werror
CC=cc

all: $(NAME)

$(NAME): $(OBJECT)
	@ar -crs $(NAME) $(OBJECT)

clean:
	@rm -f $(OBJECT)

fclean: clean
	@rm -f $(NAME)

re: fclean all

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $^ -I../include
