/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:25:58 by afelger           #+#    #+#             */
/*   Updated: 2025/03/24 14:40:18 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static long check_for_var(t_dyn_str *res, char *str, int in_quotes[2])
{
	char *var_start;
	char var_name[256];

	if ((*str == '\'' && !in_quotes[1]) || (*str == '"'
		&& !in_quotes[0]))
		return (1);
	if (*str == '$' && !in_quotes[0] && !in_quotes[1])
	{
		var_start = ++str;
		while(*str && (ft_isalnum(*str) || *str == '_'))
			str++;
		ft_memcpy(var_name, var_start, str-var_start + 1);
		dyn_str_addstr(res, ms_get_env(var_name));
		return (ft_strlen(var_name) + 1);
	}
	dyn_str_addchar(res, *str);
	return (1);
}

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
		ctr += check_for_var(result, &str[ctr], in_quotes);
	}
	tmp = result->str;
	free(result);
	return (tmp);
}
