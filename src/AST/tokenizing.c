/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:32:08 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/02/27 17:25:40 by ckrasniq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// Helper functions
char	*ft_strcpy(char * dst, const char * src)
{
	int i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

Lexer	*init_lexer(char *input)
{
	Lexer	*lexer;

	lexer = malloc(sizeof(Lexer));
	if (!lexer)
		return (NULL);
	lexer->input = input;
	lexer->input_len = ft_strlen(input);
	lexer->pos = 0;
	lexer->in_dd_quote = false;
	lexer->in_s_quote = false;
	lexer->contains_var = false;
	return (lexer);
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
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
	if (value)
		token->value = ft_strdup(value);
	else
		token->value = NULL;
	token->next = NULL;
	return (token);
}

// New function to handle variable expansion
char	*handle_variable(Lexer *lexer)
{
	char	buffer[4096] = {0};
	int		i;
	bool	has_braces;
	char	*result;

	i = 0;
	// Skip the $ character
	advance(lexer);
	// Handle special case $?
	if (current_char(lexer) == '?')
	{
		advance(lexer);
		return (ft_strdup("$?"));
	}
	// Check if we have ${VAR} format
	has_braces = false;
	if (current_char(lexer) == '{')
	{
		has_braces = true;
		advance(lexer);
	}
	// Get the variable name
	while (current_char(lexer) != '\0')
	{
		// Variable names can only contain alphanumeric chars and underscores
		if (!ft_isalnum(current_char(lexer)) && current_char(lexer) != '_')
		{
			// If we had opening brace, we need to find closing one
			if (has_braces && current_char(lexer) == '}')
			{
				advance(lexer); // Skip closing brace
				break ;
			}
			else if (has_braces)
			{
				// Error: unclosed brace
				// For now we'll just stop processing
				break ;
			}
			else
			{
				// End of variable name
				break ;
			}
		}
		buffer[i++] = current_char(lexer);
		advance(lexer);
	}
	// Prepend the $ to indicate it's a variable
	result = malloc(i + 2);
	if (!result)
		return (NULL);
	result[0] = '$';
	strcpy(result + 1, buffer);
	return (result);
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
	char	*var_name;

	quote_char = current_char(lexer);
	i = 0;
	// Add the opening quote to buffer
	buffer[i++] = quote_char;
	advance(lexer);
	// Process everything until the closing quote
	while (current_char(lexer) != '\0' && current_char(lexer) != quote_char)
	{
		// Handle variable expansion inside double quotes
		if (quote_char == '"' && current_char(lexer) == '$')
		{
			var_name = handle_variable(lexer);
			if (var_name)
			{
				ft_strcpy(buffer + i, var_name);
				i += ft_strlen(var_name);
				free(var_name);
				continue ;
			}
		}
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
	char	*var_name;
	char	quote_char;

	i = 0;
	// Handle normal words, including quoted sections and variables
	while (current_char(lexer) != '\0')
	{
		// Stop on whitespace or non-$ operators
		if (ft_isspace(current_char(lexer))
			|| (is_operator_char(current_char(lexer))
				&& current_char(lexer) != '$'))
			break ;
		// Handle quotes within words
		if (current_char(lexer) == '"' || current_char(lexer) == '\'')
		{
			quote_char = current_char(lexer);
			buffer[i++] = current_char(lexer); // Add opening quote
			advance(lexer);
			// Process everything until the closing quote
			while (current_char(lexer) != '\0'
				&& current_char(lexer) != quote_char)
			{
				// Handle variable expansion inside double quotes
				if (quote_char == '"' && current_char(lexer) == '$')
				{
					var_name = handle_variable(lexer);
					if (var_name)
					{
						strcpy(buffer + i, var_name);
						i += ft_strlen(var_name);
						free(var_name);
						continue ;
					}
				}
				buffer[i++] = current_char(lexer);
				advance(lexer);
			}
			// Add closing quote if found
			if (current_char(lexer) == quote_char)
			{
				buffer[i++] = current_char(lexer);
				advance(lexer);
			}
			continue ;
		}
		// Handle variables within words
		if (current_char(lexer) == '$')
		{
			var_name = handle_variable(lexer);
			if (var_name)
			{
				ft_strcpy(buffer + i, var_name);
				i += ft_strlen(var_name);
				free(var_name);
				lexer->contains_var = true;
				continue ;
			}
		}
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
	while (current_char(lexer) != '\0' && ft_isspace(current_char(lexer)))
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
	// Only pipe, redirect operators should be treated as operators
	// $ is not a separate operator
	if (current_char(lexer) == '|' || current_char(lexer) == '<'
		|| current_char(lexer) == '>')
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
