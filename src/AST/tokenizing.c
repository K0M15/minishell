/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:32:08 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/01 17:10:17 by ckrasniq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Function to tokenize entire input
t_token	*tokenize(char *input)
{
	t_lexer	*lexer;
	t_token	*head;
	t_token	*current;
	t_token	*token;

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

char	*handle_variable(t_lexer *lexer)
{
	char	buffer[4096] = {0};
	int		i;
	bool	has_braces;
	char	*result;

	/*================================== COMMENT AF
		is this needed? we do it in Lexing
	*/
	i = 0;
	advance(lexer);
	if (current_char(lexer) == '?')
	{
		advance(lexer);
		return (ft_strdup("$?"));
	}
	has_braces = false;
	if (current_char(lexer) == '{')
	{
		has_braces = true;
		advance(lexer);
	}
	while (current_char(lexer) != '\0')
	{
		if (!ft_isalnum(current_char(lexer)) && current_char(lexer) != '_')
		{
			if (has_braces && current_char(lexer) == '}')
			{
				advance(lexer);
				break ;
			}
			else if (has_braces)
			{
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
	result = malloc(i + 2);
	if (!result)
		return (NULL);
	result[0] = '$';
	ft_strcpy(result + 1, buffer);
	return (result);
}


// Function to handle operators
// t_token	*handle_operator(t_lexer *lexer)
// {
// 	t_token	*token;
// 	char	curr;
// 	char	next;

// 	curr = current_char(lexer);
// 	next = peek_next(lexer);
// 	if (curr == '|')
// 	{
// 		token = create_token(TOKEN_PIPE, "|");
// 		advance(lexer);
// 	}
// 	else if (curr == '<')
// 	{
// 		if (next == '<')
// 		{
// 			token = create_token(TOKEN_HERE_DOCUMENT, "<<");
// 			advance(lexer);
// 			advance(lexer);
// 		}
// 		else
// 		{
// 			token = create_token(TOKEN_REDIRECT_IN, "<");
// 			advance(lexer);
// 		}
// 	}
// 	else if (curr == '>')
// 	{
// 		if (next == '>')
// 		{
// 			token = create_token(TOKEN_APPEND_OUT, ">>");
// 			advance(lexer);
// 			advance(lexer);
// 		}
// 		else
// 		{
// 			token = create_token(TOKEN_REDIRECT_OUT, ">");
// 			advance(lexer);
// 		}
// 	}
// 	else
// 	{
// 		token = NULL;
// 	}
// 	return (token);
// }

// Function to handle quoted strings
t_token	*handle_quote(t_lexer *lexer)
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
t_token	*handle_word(t_lexer *lexer)
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
