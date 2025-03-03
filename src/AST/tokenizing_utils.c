/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:40:36 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/01 16:41:37 by ckrasniq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper functions
char	*ft_strcpy(char *dst, const char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
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
