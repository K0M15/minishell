/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniqi <ckrasniqi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 17:35:15 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/09 17:53:35 by ckrasniqi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*handle_word(t_lexer *lexer)
{
	char buffer[4096] = {0};
	int i;

	i = 0;
	while (current_char(lexer) != '\0')
	{
		if (ft_isspace(current_char(lexer))
			|| (is_operator_char(current_char(lexer))
				&& current_char(lexer) != '$'))
			break ;
		if (current_char(lexer) == '"' || current_char(lexer) == '\'')
		{
			process_quotes_in_word(lexer, buffer, &i);
			continue ;
		}
		if (current_char(lexer) == '$')
		{
			process_variable_in_word(lexer, buffer, &i);
			continue ;
		}
		buffer[i++] = current_char(lexer);
		advance(lexer);
	}
	if (i == 0)
		return (NULL);
	return (create_token(TOKEN_WORD, buffer));
}

void	process_variable_in_word(t_lexer *lexer, char *buffer, int *i)
{
	char	*var_name;

	var_name = handle_variable(lexer);
	if (var_name)
	{
		ft_strcpy(buffer + *i, var_name);
		*i += ft_strlen(var_name);
		free(var_name);
		lexer->contains_var = true;
	}
}

void process_quotes_in_word(t_lexer *lexer, char *buffer, int *i)
{
    char quote_char;

    quote_char = current_char(lexer);
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
