.PHONY = clean fclean re

PATH_LIBFT = libft/libft.a
FLAGS = -Wall -Wextra -Werror
FLAGS += -g -fsanitize=address
# FLAGS += -lreadline
NAME = minishell
SRC_DIR = src
#   CORE
FILES=minishell.c
#	RUNTIME
FILES += enviroment/enviroment.c appstate.c
FILES += signals/signals.c
#	BUILTINS
FILES += builtin/pwd/pwd.c builtin/cd/cd.c builtin/export/export.c builtin/env/env.c
FILES += builtin/echo/echo.c builtin/unset/unset.c builtin/exit/exit.c
#	PROMPT
FILES += prompt/prompt.c
F_INC = -Iinclude
CC = cc
# CC = gcc

# Directory for object files
OBJ_DIR = objects
# FILES_SRC = $(patsubst %.c,$(SRC_DIR)/%.c,$(FILES))
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(FILES))

all: $(OBJ_DIR) $(NAME)

# Create object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/builtin/pwd
	mkdir -p $(OBJ_DIR)/builtin/export
	mkdir -p $(OBJ_DIR)/builtin/unset
	mkdir -p $(OBJ_DIR)/builtin/cd
	mkdir -p $(OBJ_DIR)/builtin/echo
	mkdir -p $(OBJ_DIR)/builtin/exit
	mkdir -p $(OBJ_DIR)/builtin/env
	mkdir -p $(OBJ_DIR)/enviroment
	mkdir -p $(OBJ_DIR)/signals
	mkdir -p $(OBJ_DIR)/prompt

$(NAME): $(OBJ_FILES) libft
	$(CC) $(OBJ_FILES) -o $(NAME) $(FLAGS) $(PATH_LIBFT) -lreadline

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(FLAGS) -c -o $@ $< $(F_INC)

libft: $(PATH_LIBFT)

$(PATH_LIBFT):
	make -C libft bonus

clean:
	make -C libft clean
	rm -rf $(OBJ_DIR)

fclean: clean
	make -C libft fclean
	rm -f $(NAME)

re: fclean all
