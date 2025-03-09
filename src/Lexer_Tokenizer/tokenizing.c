/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniqi <ckrasniqi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:32:08 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/09 17:48:25 by ckrasniqi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token_to_list(t_token **head, t_token **current, t_token *token)
{
	if (*head == NULL)
	{
		*head = token;
		*current = token;
	}
	else
	{
		(*current)->next = token;
		*current = token;
	}
}

t_token	*build_token_list(t_lexer *lexer)
{
	t_token	*head;
	t_token	*current;
	t_token	*token;

	head = NULL;
	current = NULL;
	while (1)
	{
		token = get_next_token(lexer);
		if (!token)
			break ;
		add_token_to_list(&head, &current, token);
		if (token->type == TOKEN_EOF)
			break ;
	}
	return (head);
}

t_token	*tokenize(char *input)
{
	t_lexer	*lexer;
	t_token	*head;

	lexer = init_lexer(input);
	if (!lexer)
		return (NULL);
	head = build_token_list(lexer);
	free(lexer);
	return (head);
}

void	process_variable_name(t_lexer *lexer, char *buffer, int *i, bool has_braces)
{
	while (current_char(lexer) != '\0')
	{
		if (!ft_isalnum(current_char(lexer)) && current_char(lexer) != '_')
		{
			if (has_braces && current_char(lexer) == '}')
			{
				advance(lexer);
				break ;
			}
			else if (has_braces || !has_braces)
			{
				break ;
			}
		}
		buffer[(*i)++] = current_char(lexer);
		advance(lexer);
	}
}

char	*handle_variable(t_lexer *lexer)
{
	char	buffer[4096] = {0};
	int		i;
	bool	has_braces;
	char	*result;

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
	process_variable_name(lexer, buffer, &i, has_braces);
	result = malloc(i + 2);
	if (!result)
		return (NULL);
	result[0] = '$';
	ft_strcpy(result + 1, buffer);
	return (result);
}
