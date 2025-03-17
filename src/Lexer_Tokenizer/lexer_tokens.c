/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 17:24:21 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/17 13:28:32 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token	*create_token(t_tokentype type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
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

t_token	*get_next_token(t_lexer *lexer)
{
	char	current;

	skip_whitespace(lexer);
	current = current_char(lexer);
	if (current == '\0')
	{
		return (create_token(TOKEN_EOF, NULL));
	}
	if (current == '|' || current == '<'
		|| current == '>')
	{
		return (handle_operator(lexer));
	}
	return (handle_word(lexer));
}

void	skip_whitespace(t_lexer *lexer)
{
	while (current_char(lexer) != '\0' && ft_isspace(current_char(lexer)))
		advance(lexer);
}

void	free_tokens(t_token *head)
{
	t_token *temp;

	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
}
