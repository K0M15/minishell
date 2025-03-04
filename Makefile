.PHONY = clean fclean re

PATH_LIBFT = libft/libft.a
PATH_FT_DYN_STR = ft_dyn_str/ft_dyn_str.a
FLAGS = -Wall -Wextra -Werror
FLAGS += -g -fsanitize=address
# FLAGS += -lreadline
NAME = minishell
SRC_DIR = src
#   CORE
FILES = minishell.c
#	PARSING
FILES += AST/tokenizing.c AST/tokenizing_handler.c AST/tokenizing_init.c AST/tokenizing_utils.c
FILES += AST/variables_expanding.c AST/redirection.c AST/redirection_utils.c AST/parsing.c
FILES += AST/AST_helper_functions.c AST/AST_utils.c AST/execution.c
#	RUNTIME
FILES += enviroment/enviroment.c state/appstate.c
FILES += signals/signals.c
#	BUILTINS
FILES += builtin/pwd/pwd.c builtin/cd/cd.c builtin/export/export.c builtin/env/env.c
FILES += builtin/echo/echo.c builtin/unset/unset.c builtin/exit/exit.c builtin/builtin.c
#	PROMPT
FILES += prompt/prompt.c prompt/history.c prompt/terminal.c prompt/heredoc.c prompt/heredoc_util.c
#	MEMORY
FILES += mem_manager/ft_malloc.c

F_INC = -I ./include
CC = cc
# CC = gcc

# Directory for object files
OBJ_DIR = objects
# FILES_SRC = $(patsubst %.c,$(SRC_DIR)/%.c,$(FILES))
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(FILES))

all: $(OBJ_DIR) $(NAME)

#Test execute simple command
tsimple: $(OBJ_DIR) $(OBJ_FILES)
	cc tests/test_simple_cmd.c $(filter-out objects/minishell.o, $(OBJ_FILES)) $(F_INC) -o tests/test_simple_cmd $(FLAGS) $(PATH_LIBFT) -lreadline
	# time tests/test_simple_cmd

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
	@mkdir -p $(OBJ_DIR)/AST
	@mkdir -p $(OBJ_DIR)/state

$(NAME): $(OBJ_FILES) $(PATH_LIBFT) $(PATH_FT_DYN_STR)
	@$(CC) $(OBJ_FILES) -o $(NAME) $(FLAGS) $(PATH_LIBFT) $(PATH_FT_DYN_STR) -lreadline

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(FLAGS) -c -o $@ $< $(F_INC)

# libft: $(PATH_LIBFT)

$(PATH_FT_DYN_STR):
	@make -C ft_dyn_str

$(PATH_LIBFT):
	@make -C libft bonus

clean:
	@make -C libft clean
	@make -C ft_dyn_str clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@make -C libft fclean
	@make -C ft_dyn_str fclean
	@rm -f $(NAME)

re: fclean all
