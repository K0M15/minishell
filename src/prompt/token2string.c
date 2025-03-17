/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:00:29 by afelger           #+#    #+#             */
/*   Updated: 2025/03/17 14:06:58 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*look_token_rep(t_token *token)
{
	if (token->type == TOKEN_WORD)
		return (token->value);
	else if (TOKEN_PIPE)
		return ("pipeline");
	else if (TOKEN_REDIRECT_IN)
		return ("<");
	else if (TOKEN_REDIRECT_OUT)
		return (">");
	else if (TOKEN_APPEND_OUT)
		return (">>");
	else if (TOKEN_HERE_DOCUMENT)
		return ("<<");
	else if (TOKEN_EOF)
		return ("newline");
	else
		return ("NULL");
}
