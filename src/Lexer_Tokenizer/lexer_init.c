/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniqi <ckrasniqi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 17:20:58 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/09 17:23:28 by ckrasniqi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*init_lexer(char *input)
{
	t_lexer	*lexer;

	lexer = malloc(sizeof(t_lexer));
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

char	current_char(t_lexer *lexer)
{
	if (lexer->pos >= lexer->input_len)
		return ('\0');
	return (lexer->input[lexer->pos]);
}

char	peek_next(t_lexer *lexer)
{
	if (lexer->pos + 1 >= lexer->input_len)
		return ('\0');
	return (lexer->input[lexer->pos + 1]);
}

void	advance(t_lexer *lexer)
{
	lexer->pos++;
}
