.PHONY = clean fclean re

PATH_LIBFT = libft/libft.a
FLAGS = -Wall -Wextra -Werror
FLAGS += -g -fsanitize=address
# FLAGS += -lreadline
NAME = minishell
SRC_DIR = src
#   CORE
FILES=minishell.c
#	PARSING
FILES += tokenizing.c
#	RUNTIME
FILES += enviroment/enviroment.c appstate.c
FILES += signals/signals.c
#	BUILTINS
FILES += builtin/pwd/pwd.c builtin/cd/cd.c builtin/export/export.c builtin/env/env.c
FILES += builtin/echo/echo.c builtin/unset/unset.c builtin/exit/exit.c
#	PROMPT
FILES += prompt/prompt.c prompt/history.c prompt/terminal.c prompt/heredoc.c prompt/heredoc_util.c
#	MEMORY
FILES += mem_manager/ft_malloc.c
#	EXECUTION
FILES += execution/execute.c

F_INC = -I ./include
CC = cc
# CC = gcc

# Directory for object files
OBJ_DIR = objects
# FILES_SRC = $(patsubst %.c,$(SRC_DIR)/%.c,$(FILES))
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(FILES))

all: $(OBJ_DIR) $(NAME)

#Test execution
texe: $(OBJ_DIR) $(OBJ_FILES)
	cc tests/test_run_command.c $(filter-out objects/minishell.o, $(OBJ_FILES)) $(F_INC) -o tests/test_run_command $(FLAGS) $(PATH_LIBFT) -lreadline
	time tests/test_run_command

theredoc: $(OBJ_DIR) $(OBJ_FILES)
	cc tests/test_run_heredoc.c $(filter-out objects/minishell.o, $(OBJ_FILES)) $(F_INC) -o tests/test_run_heredoc $(FLAGS) $(PATH_LIBFT) -lreadline
	time tests/test_run_heredoc

# Create object directory if it doesn't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/builtin/pwd
	@mkdir -p $(OBJ_DIR)/builtin/export
	@mkdir -p $(OBJ_DIR)/builtin/unset
	@mkdir -p $(OBJ_DIR)/builtin/cd
	@mkdir -p $(OBJ_DIR)/builtin/echo
	@mkdir -p $(OBJ_DIR)/builtin/exit
	@mkdir -p $(OBJ_DIR)/builtin/env
	@mkdir -p $(OBJ_DIR)/enviroment
	@mkdir -p $(OBJ_DIR)/execution
	@mkdir -p $(OBJ_DIR)/signals
	@mkdir -p $(OBJ_DIR)/prompt
	@mkdir -p $(OBJ_DIR)/mem_manager

$(NAME): $(OBJ_FILES) libft
	@$(CC) $(OBJ_FILES) -o $(NAME) $(FLAGS) $(PATH_LIBFT) -lreadline

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(FLAGS) -c -o $@ $< $(F_INC)

libft: $(PATH_LIBFT)

$(PATH_LIBFT):
	@make -C libft bonus

clean:
	@make -C libft clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@make -C libft fclean
	@rm -f $(NAME)

re: fclean all
