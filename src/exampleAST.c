/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exampleAST.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:27:31 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/02/19 19:36:11 by ckrasniq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <ctype.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
}					Lexer;

// Helper functions
Lexer	*init_lexer(char *input)
{
	Lexer	*lexer;

	lexer = malloc(sizeof(Lexer));
	if (!lexer)
		return (NULL);
	lexer->input = input;
	lexer->input_len = strlen(input);
	lexer->pos = 0;
	lexer->in_dd_quote = false;
	lexer->in_s_quote = false;
	return (lexer);
}

char	current_char(Lexer *lexer)
{
	if (lexer->pos >= lexer->input_len)
		return ('\0');
	return (lexer->input[lexer->pos]);
}

char	peek_next(Lexer *lexer)
{
	if (lexer->pos + 1 >= lexer->input_len)
		return ('\0');
	return (lexer->input[lexer->pos + 1]);
}

void	advance(Lexer *lexer)
{
	lexer->pos++;
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

// Function to create a new token
Token	*create_token(TokenType type, char *value)
{
	Token	*token;

	token = malloc(sizeof(Token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value ? strdup(value) : NULL;
	token->next = NULL;
	return (token);
}

// Function to handle operators
Token	*handle_operator(Lexer *lexer)
{
	Token	*token;
	char	curr;
	char	next;

	curr = current_char(lexer);
	next = peek_next(lexer);
	if (curr == '|')
	{
		token = create_token(TOKEN_PIPE, "|");
		advance(lexer);
	}
	else if (curr == '<')
	{
		if (next == '<')
		{
			token = create_token(TOKEN_HERE_DOCUMENT, "<<");
			advance(lexer);
			advance(lexer);
		}
		else
		{
			token = create_token(TOKEN_REDIRECT_IN, "<");
			advance(lexer);
		}
	}
	else if (curr == '>')
	{
		if (next == '>')
		{
			token = create_token(TOKEN_APPEND_OUT, ">>");
			advance(lexer);
			advance(lexer);
		}
		else
		{
			token = create_token(TOKEN_REDIRECT_OUT, ">");
			advance(lexer);
		}
	}
	else
	{
		token = NULL;
	}
	return (token);
}

// Function to handle quoted strings
Token	*handle_quote(Lexer *lexer)
{
	char	quote_char;
	char	buffer[4096] = {0};
	int		i;

	quote_char = current_char(lexer);
	i = 0;
	// Add the opening quote to buffer
	buffer[i++] = quote_char;
	advance(lexer);
	// Process everything until the closing quote
	while (current_char(lexer) != '\0' && current_char(lexer) != quote_char)
	{
		buffer[i++] = current_char(lexer);
		advance(lexer);
	}
	// Add the closing quote if found
	if (current_char(lexer) == quote_char)
	{
		buffer[i++] = quote_char;
		advance(lexer);
	}
	return (create_token(TOKEN_WORD, buffer));
}

// Improved function to handle words and quotations
Token	*handle_word(Lexer *lexer)
{
	char	buffer[4096] = {0};
	int		i;

	i = 0;
	// If we start with a quote character, handle it separately
	if (current_char(lexer) == '"' || current_char(lexer) == '\'')
	{
		return (handle_quote(lexer));
	}
	// Handle normal words
	while (current_char(lexer) != '\0')
	{
		// Stop on whitespace or operators
		if (isspace(current_char(lexer))
			|| is_operator_char(current_char(lexer)))
			break ;
		// Stop when we hit a quote character - let it be processed separately
		if (current_char(lexer) == '"' || current_char(lexer) == '\'')
			break ;
		buffer[i++] = current_char(lexer);
		advance(lexer);
	}
	if (i == 0)
		return (NULL);
	return (create_token(TOKEN_WORD, buffer));
}

// Function to skip whitespace
void	skip_whitespace(Lexer *lexer)
{
	while (current_char(lexer) != '\0' && isspace(current_char(lexer)))
		advance(lexer);
}

// Main tokenizing function
Token	*get_next_token(Lexer *lexer)
{
	skip_whitespace(lexer);
	if (current_char(lexer) == '\0')
	{
		return (create_token(TOKEN_EOF, NULL));
	}
	if (is_operator_char(current_char(lexer)))
	{
		return (handle_operator(lexer));
	}
	return (handle_word(lexer));
}

// Function to tokenize entire input
Token	*tokenize(char *input)
{
	Lexer	*lexer;
	Token	*head;
	Token	*current;
	Token	*token;

	head = NULL;
	current = NULL;
	lexer = init_lexer(input);
	if (!lexer)
		return (NULL);
	while (1)
	{
		token = get_next_token(lexer);
		if (!token)
			break ;
		if (head == NULL)
		{
			head = token;
			current = token;
		}
		else
		{
			current->next = token;
			current = token;
		}
		if (token->type == TOKEN_EOF)
			break ;
	}
	free(lexer);
	return (head);
}

// Helper function to free token list
void	free_tokens(Token *head)
{
	Token	*temp;

	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
}

void	print_token_type(TokenType type)
{
	if (type == TOKEN_WORD)
		printf("WORD");
	else if (type == TOKEN_PIPE)
		printf("PIPE");
	else if (type == TOKEN_REDIRECT_IN)
		printf("REDIRECT_IN");
	else if (type == TOKEN_REDIRECT_OUT)
		printf("REDIRECT_OUT");
	else if (type == TOKEN_APPEND_OUT)
		printf("APPEND_OUT");
	else if (type == TOKEN_HERE_DOCUMENT)
		printf("HERE_DOCUMENT");
	else if (type == TOKEN_EOF)
		printf("EOF");
}

int	main(void)
{
	char *input;
	Token *tokens;
	Token *current;

	while (1)
	{
		input = readline("minishell: ");
		if (!input) // Handle Ctrl+D
			break ;
		tokens = tokenize(input);
		current = tokens;
		while (current)
		{
			printf("Type: ");
			print_token_type(current->type);
			if (current->value)
				printf(", Value: %s \n", current->value);
			else
				printf("\n");
			current = current->next;
		}
		free_tokens(tokens);
		free(input);
	}
	return (0);
}