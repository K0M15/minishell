/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:43:47 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/01 16:44:20 by ckrasniq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*handle_pipe(t_lexer *lexer)
{
	advance(lexer);
	return (create_token(TOKEN_PIPE, "|"));
}

t_token	*handle_redirect_in(t_lexer *lexer)
{
	if (peek_next(lexer) == '<')
	{
		advance(lexer);
		advance(lexer);
		return (create_token(TOKEN_HERE_DOCUMENT, "<<"));
	}
	advance(lexer);
	return (create_token(TOKEN_REDIRECT_IN, "<"));
}

t_token	*handle_redirect_out(t_lexer *lexer)
{
	if (peek_next(lexer) == '>')
	{
		advance(lexer);
		advance(lexer);
		return (create_token(TOKEN_APPEND_OUT, ">>"));
	}
	advance(lexer);
	return (create_token(TOKEN_REDIRECT_OUT, ">"));
}

t_token	*handle_operator(t_lexer *lexer)
{
	char	curr;

	curr = current_char(lexer);
	if (curr == '|')
		return (handle_pipe(lexer));
	else if (curr == '<')
		return (handle_redirect_in(lexer));
	else if (curr == '>')
		return (handle_redirect_out(lexer));
	return (NULL);
}

void	free_tokens(t_token *head)
{
	t_token	*temp;

	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
}