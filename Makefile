# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/16 16:33:15 by ckrasniq          #+#    #+#              #
#    Updated: 2025/02/19 19:51:53 by ckrasniq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY = clean fclean re

PATH_LIBFT = libft/libft.a
FLAGS = -Wall -Wextra -Werror
FLAGS += -g -fsanitize=address

# FLAGS += -lreadline
NAME = minishell
SRC_DIR = src
#   CORE
FILES = minishell.c
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
	@mkdir -p $(OBJ_DIR)/src

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