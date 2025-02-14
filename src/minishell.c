/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:59:31 by afelger           #+#    #+#             */
/*   Updated: 2025/02/14 16:12:55 by ckrasniq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TOKENS 100

typedef enum
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND_OUT,
	TOKEN_HERE_DOCUMENT,
	TOKEN_DELIMITER,
	TOKEN_EOF,

	// other token types...
}				TokenType;

typedef struct
{
	TokenType	type;
	char		*value;
}				Token;

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

void	handle_pipe(char *input, Token *tokens, int *token_count, int *i)
{
	// Handle pipe...
	if (input[*i] == '|')
	{
		tokens[*token_count].type = TOKEN_PIPE;
		tokens[*token_count].value = ft_strdup("|");
		(*token_count)++;
	}
}

void	handle_word(char *input, Token *tokens, int *token_count, int *i)
{
	int	start;
	int	len;

	// Handle words
	if (ft_isalnum(input[*i]))
	{
		start = *i;
		while (ft_isalnum(input[*i]))
			(*i)++;
		len = *i - start;
		tokens[*token_count].type = TOKEN_WORD;
		tokens[*token_count].value = ft_strndup(input + start, len);
		(*token_count)++;
	}
}
void	handle_arrows(char *input, Token *tokens, int *token_count, int *i)
{
	int	start;
	int	len;

	// Handle redirection...
	if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			tokens[*token_count].type = TOKEN_APPEND_OUT;
			tokens[*token_count].value = ft_strdup(">>");
			(*i)++; // Skip the next character
		}
		else
		{
			tokens[*token_count].type = TOKEN_REDIRECT_OUT;
			tokens[*token_count].value = ft_strdup(">");
		}
		(*token_count)++;
	}
	if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			tokens[*token_count].type = TOKEN_HERE_DOCUMENT;
			tokens[*token_count].value = ft_strdup("<<");
			(*i) += 2;
			(*token_count)++;
			while (is_whitespace(input[*i]))
				(*i)++;
			start = *i;
			while (!is_whitespace(input[*i]))
				(*i)++;
			len = *i - start;
			tokens[*token_count].type = TOKEN_DELIMITER;
			tokens[*token_count].value = ft_strndup(input + start, len);
			(*i)++; // Skip the next character
		}
		else
		{
			tokens[*token_count].type = TOKEN_REDIRECT_IN;
			tokens[*token_count].value = ft_strdup("<");
		}
		(*token_count)++;
	}
}

Token	*tokenize(char *input)
{
	Token	*tokens;
	int		token_count;
	int		i;

	i = 0;
	token_count = 0;
	// Allocate space for tokens
	tokens = malloc(sizeof(Token) * MAX_TOKENS);
	if (!tokens)
		return (NULL);
	// Iterate through input
	while (input[i] != '\0')
	{
		// Skip whitespace
		if (is_whitespace(input[i]))
		{
			i++;
			continue ;
		}
		// Handle operators
		if (input[i] == '|' || input[i] == '>' || input[i] == '<')
		{
			// Handle pipe...
			if (input[i] == '|')
				handle_pipe(input, tokens, &token_count, &i);
			// Handle redirection...
			if (input[i] == '>' || input[i] == '<')
				handle_arrows(input, tokens, &token_count, &i);
			// (more complex logic here)
		}
		// Handle words
		if (ft_isalnum(input[i]))
			handle_word(input, tokens, &token_count, &i);
		// (more complex logic here)
		else
			i++;
	}
	// Mark end of tokens
	tokens[token_count].type = TOKEN_EOF;
	return (tokens);
}
int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: %s <input>\n", argv[0]);
		return (1);
	}
	Token *tokens = tokenize(argv[1]);
	int i = 0;
	while (tokens[i].type != TOKEN_EOF)
	{
		printf("Token %d: type=%d, value=%s\n", i, tokens[i].type,
			tokens[i].value);
		i++;
	}
	return (0);
}