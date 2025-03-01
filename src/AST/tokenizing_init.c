/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:42:04 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/01 16:42:45 by ckrasniq         ###   ########.fr       */
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

void	skip_whitespace(t_lexer *lexer)
{
	while (current_char(lexer) != '\0' && ft_isspace(current_char(lexer)))
		advance(lexer);
}

t_token	*get_next_token(t_lexer *lexer)
{
	skip_whitespace(lexer);
	if (current_char(lexer) == '\0')
	{
		return (create_token(TOKEN_EOF, NULL));
	}
	// Only pipe, redirect operators should be treated as operators
	// $ is not a separate operator
	if (current_char(lexer) == '|' || current_char(lexer) == '<'
		|| current_char(lexer) == '>')
	{
		return (handle_operator(lexer));
	}
	return (handle_word(lexer));
}