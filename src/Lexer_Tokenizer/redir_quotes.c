/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:25:58 by afelger           #+#    #+#             */
/*   Updated: 2025/03/18 15:29:11 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_quotes_redir(char *str)
{
	t_dyn_str	*result;
	long		ctr;
	int			in_quotes[2] = {0, 0};
	char		*tmp;

	result = dyn_str_new();
	ctr = 0;
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
	free(result);
	return (tmp);
}