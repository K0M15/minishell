/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:25:58 by afelger           #+#    #+#             */
/*   Updated: 2025/03/29 11:52:19 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static long	check_for_var(t_dyn_str *res, char *str, int in_quotes[2])
{
	if ((*str == '\'' && !in_quotes[1]) || (*str == '"'
			&& !in_quotes[0]))
		return (1);
	dyn_str_addchar(res, *str);
	return (1);
}

char	*handle_quotes_redir(char *str)
{
	t_dyn_str	*result;
	long		ctr;
	int			in_quotes[2];
	char		*tmp;

	result = dyn_str_new();
	ctr = 0;
	ft_memset(in_quotes, 0, sizeof(int) * 2);
	while (str[ctr])
	{
		handle_quote_state(str[ctr], in_quotes);
		ctr += check_for_var(result, &str[ctr], in_quotes);
	}
	tmp = result->str;
	ft_free(result);
	return (tmp);
}
