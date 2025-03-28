.PHONY = clean fclean re

PATH_LIBFT = libft/libft.a
PATH_FT_DYN_STR = ft_dyn_str/ft_dyn_str.a

# Compiler flags
FLAGS = -Wall -Wextra -Werror
FLAGS += -g
FLAGS += -fsanitize=address

# Readline flags
# READLINE_PATH := $(shell brew --prefix readline)
# LDFLAGS = -L$(READLINE_PATH)/lib
# CFLAGS = -I$(READLINE_PATH)/include
LIBS = -lreadline
# FLAGS += -lreadline
NAME = minishell
SRC_DIR = src
#   CORE
FILES = minishell.c
#	LEXER
FILES += Lexer_Tokenizer/lexer_init.c Lexer_Tokenizer/lexer_operators.c Lexer_Tokenizer/lexer_tokens.c
FILES += Lexer_Tokenizer/lexer_words.c Lexer_Tokenizer/lexer_quotes.c Lexer_Tokenizer/tokenizing.c
FILES += Lexer_Tokenizer/redir_quotes.c
#	AST
# Add to FILES under #AST section:
FILES += AST/variables_expanding.c AST/redirections.c AST/string_utils.c AST/var_utils.c 
FILES += AST/parser_commands.c AST/parser_redirections.c AST/memory_utils.c AST/parser_utils.c
FILES += AST/utils.c AST/executor_utils.c AST/executor.c AST/pipe_utils.c
FILES += AST/execution_checkers.c AST/parser_redirections_2.c AST/free_cmd.c
#	RUNTIME
FILES += enviroment/enviroment.c enviroment/env_strfunc.c enviroment/env_data.c
FILES += state/appstate.c state/preprocess.c
FILES += signals/signals.c signals/sig_init.c
#	BUILTINS
FILES += builtin/pwd/pwd.c builtin/cd/cd.c builtin/export/export.c builtin/env/env.c
FILES += builtin/echo/echo.c builtin/unset/unset.c builtin/exit/exit.c builtin/builtin.c
FILES += builtin/export/sort_env.c
#	PROMPT
FILES += prompt/prompt.c prompt/history.c prompt/terminal.c prompt/heredoc.c prompt/heredoc_util.c prompt/token2string.c
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
	@mkdir -p $(OBJ_DIR)/Lexer_Tokenizer
	@mkdir -p $(OBJ_DIR)/state

$(NAME): $(OBJ_FILES) $(PATH_LIBFT) $(PATH_FT_DYN_STR)
	@$(CC) $(FLAGS) $(LDFLAGS) -o $(NAME) $(OBJ_FILES) $(PATH_LIBFT) $(PATH_FT_DYN_STR) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(FLAGS) $(CFLAGS) -c -o $@ $< $(F_INC)

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
