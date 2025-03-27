/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 17:35:15 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/27 14:14:28 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_token	*handle_word(t_lexer *lexer)
{
	char	buffer[4096];
	int		i;
	int		in_quotes[2];

	i = 0;
	ft_memset(buffer, 0, 4096);
	ft_memset(in_quotes, 0, 8);
	while (current_char(lexer) != '\0')
	{

		if (ft_isspace(current_char(lexer))
			|| (is_operator_char(current_char(lexer))
				&& current_char(lexer) != '$'))
			break ;
		if (current_char(lexer) == '"' || current_char(lexer) == '\'')
		{
			// in_quotes[0] = (current_char(lexer) == '"') * !in_quotes[0];
			// in_quotes[1] = (current_char(lexer) == '\'') * !in_quotes[1];
			process_quotes_in_word(lexer, buffer, &i);
			continue ;
		}
		if (current_char(lexer) == '$')
		{
	
			process_variable_in_word(lexer, buffer, &i, (int *)in_quotes); // add var extration here || quotes
	
			continue ;
		}
		buffer[i++] = current_char(lexer);
		advance(lexer);
	}
	if (i == 0)
		return (NULL);
	return (create_token(TOKEN_WORD, buffer));
}

char *process_variable(char *var_name, int in_quotes)
{
	char *value;

	if ((in_quotes && var_name) || (var_name && ft_strlencmp("$", var_name) == 0))
		value = var_name;
	else if (var_name && *(var_name + 1) != '?')
	{
		value = ft_strdup(ms_get_env(var_name + 1));
		free(var_name);
	}
	else if (var_name && *(var_name + 1) == '?')
	{
		value = ft_itoa(get_appstate()->last_return);
		free(var_name);
	}
	else
		value = ft_strdup("");
	return (value);
}

void	process_variable_in_word(t_lexer *lexer, char *buffer, int *i, int *in_quotes)
{
	char	*var_name;
	char	*var_val;

	var_name = handle_variable(lexer);
	var_val = process_variable(var_name, in_quotes[1]);
	ft_strcpy(buffer + *i, var_val);
	*i += ft_strlen(var_val);
	free(var_val);
}

void	process_quotes_in_word(t_lexer *lexer, char *buffer, int *i)
{
    char quote_char;
	// int in_quotes;

    quote_char = current_char(lexer);
	// in_quotes = (quote_char=='\"') << 8 | (quote_char=='\'');
    buffer[(*i)++] = current_char(lexer);
    advance(lexer);
    
    while (current_char(lexer) != '\0' && current_char(lexer) != quote_char)
    {
        process_variable_in_quotes(lexer, buffer, i, quote_char);
    }
    
    if (current_char(lexer) == quote_char)
    {
		buffer[(*i)++] = current_char(lexer);
        advance(lexer);
    }
}
