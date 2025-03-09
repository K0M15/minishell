/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniqi <ckrasniqi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 17:33:13 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/09 17:34:51 by ckrasniqi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
