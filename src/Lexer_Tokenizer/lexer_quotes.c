/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 17:37:22 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/29 11:44:48 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*handle_quote(t_lexer *lexer)
{
	char	quote_char;
	char	buffer[4096];
	int		i;

	ft_memset(buffer, 0, 4096);
	quote_char = current_char(lexer);
	i = 0;
	buffer[i++] = quote_char;
	advance(lexer);
	process_quote_content(lexer, buffer, &i, quote_char);
	if (current_char(lexer) == quote_char)
	{
		buffer[i++] = quote_char;
		advance(lexer);
	}
	return (create_token(TOKEN_WORD, buffer));
}

void	process_quote_content(t_lexer *lexer,
	char *buffer, int *i, char quote_char)
{
	char	*var_name;

	while (current_char(lexer) != '\0' && current_char(lexer) != quote_char)
	{
		if (quote_char == '"' && current_char(lexer) == '$')
		{
			var_name = handle_variable(lexer);
			if (var_name)
			{
				ft_strcpy(buffer + *i, var_name);
				*i += ft_strlen(var_name);
				ft_free(var_name);
				continue ;
			}
		}
		buffer[(*i)++] = current_char(lexer);
		advance(lexer);
	}
}

char	*handle_quotes(char *str)
{
	t_dyn_str	*result;
	long		ctr;
	int			in_quotes[2];
	char		*tmp;

	result = dyn_str_new();
	ctr = 0;
	ft_memset(in_quotes, 0, 8);
	while (str[ctr])
	{
		handle_quote_state(str[ctr], in_quotes);
		if ((str[ctr] == '\'' && !in_quotes[1]) || (str[ctr] == '"'
				&& !in_quotes[0]))
		{
			ctr++;
			continue ;
		}
		dyn_str_addchar(result, str[ctr]);
		ctr++;
	}
	tmp = result->str;
	ft_free(result);
	return (tmp);
}

void	handle_quote_state(char c, int *in_quotes)
{
	if (c == '\'' && !in_quotes[1])
		toggle_quote(&in_quotes[0]);
	else if (c == '"' && !in_quotes[0])
		toggle_quote(&in_quotes[1]);
}
