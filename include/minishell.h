/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:00:44 by afelger           #+#    #+#             */
/*   Updated: 2025/02/26 16:39:54 by ckrasniq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "libft.h"
#include <stdio.h>
#include <ctype.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INITIAL_ARG_CAPACITY 16

typedef enum
{
	TOKEN_WORD,          // Words and quoted strings
	TOKEN_PIPE,          // |
	TOKEN_REDIRECT_IN,   // <
	TOKEN_REDIRECT_OUT,  // >
	TOKEN_APPEND_OUT,    // >>
	TOKEN_HERE_DOCUMENT, // <<
	TOKEN_EOF            // End of input
}					TokenType;


// Redirection types
typedef struct Token
{
	TokenType		type;
	char			*value;
	struct Token	*next;
}					Token;

typedef struct Lexer
{
	char			*input;
	size_t			input_len;
	size_t			pos;
	bool			in_dd_quote;
	bool			in_s_quote;
	bool			contains_var;
}					Lexer;

// Command types
typedef enum
{
	CMD_SIMPLE, // Simple command (e.g., "ls -l")
	CMD_PIPE,   // Pipeline (e.g., "ls | grep foo")
	CMD_REDIR   // Command with redirections
}				CommandType;

typedef enum
{
	REDIR_IN,     // Input redirection (<)
	REDIR_OUT,    // Output redirection (>)
	REDIR_APPEND, // Append output (>>)
	REDIR_HEREDOC // Here document (<<)
}				RedirType;

// Redirection structure
typedef struct s_redirection
{
	RedirType				type;
	char					*file; // Target file/delimiter
	struct s_redirection	*next; // Next redirection
}				t_redirection;

// Command structure (node in the AST)
typedef struct s_command
{
	CommandType	type;

	// For simple commands
	char **args; // Command arguments (argv-style)

	// For pipelines
	struct s_command *left;  // Left side of pipe
	struct s_command *right; // Right side of pipe

	// For all commands - redirections
	t_redirection *redirections; // List of redirections
}				t_command;


// Tokenizing functions

Lexer	*init_lexer(char *input);
int		ft_isspace(char c);
char	current_char(Lexer *lexer);
char	peek_next(Lexer *lexer);
void	advance(Lexer *lexer);
int		is_operator_char(char c);
Token	*create_token(TokenType type, char *value);
char	*handle_variable(Lexer *lexer);
Token	*handle_operator(Lexer *lexer);
Token	*handle_word(Lexer *lexer);
Token	*handle_quote(Lexer *lexer);
void	skip_whitespace(Lexer *lexer);
Token	*get_next_token(Lexer *lexer);
Token	*tokenize(char *input);
void	free_tokens(Token *tokens);
void	print_token_type(TokenType type);

#endif // MINISHELL_H