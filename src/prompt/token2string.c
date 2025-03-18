/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:00:29 by afelger           #+#    #+#             */
/*   Updated: 2025/03/18 14:56:07 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*look_token_rep(t_token *token)
{
	if (token->type == TOKEN_WORD)
		return (token->value);
	else if (token->type == TOKEN_PIPE)
		return ("pipeline");
	else if (token->type == TOKEN_REDIRECT_IN)
		return ("<");
	else if (token->type == TOKEN_REDIRECT_OUT)
		return (">");
	else if (token->type == TOKEN_APPEND_OUT)
		return (">>");
	else if (token->type == TOKEN_HERE_DOCUMENT)
		return ("<<");
	else if (token->type == TOKEN_EOF)
		return ("newline");
	else
		return ("NULL");
}
