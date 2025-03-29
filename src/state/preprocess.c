/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:31:48 by afelger           #+#    #+#             */
/*   Updated: 2025/03/29 11:41:38 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_single(int *offset, char **str, t_dyn_str *res, int *inquote)
{
	if (*offset == 1)
	{
		if ((*str)[1] != '"' && (*str)[1] != '\'')
			dyn_str_addchar(res, **str);
		else if (inquote[0] || inquote[1])
			dyn_str_addchar(res, **str);
		(*str)++;
		return (1);
	}
	return (0);
}

void	handle_dollars(t_dyn_str *res, char **str, int *inquote)
{
	int		offset;
	char	*var_name;

	offset = 1;
	while ((*str)[offset] && !ft_isspace((*str)[offset])
		&& (ft_isalnum((*str)[offset]) || (*str)[offset] == '_'
		|| (*str)[offset] == '?'))
		offset++;
	if (handle_single(&offset, str, res, inquote))
		return ;
	var_name = malloc(offset);
	ft_strndup(var_name, (*str) + 1, offset - 1);
	if (var_name[0] == '?')
	{
		free(var_name);
		var_name = ft_itoa(get_appstate()->last_return);
		dyn_str_addstr(res, var_name);
		offset = 2;
	}
	else
		dyn_str_addstr(res, ms_get_env(var_name));
	free(var_name);
	(*str) += offset;
}

void	handle_next_quote(int *inquote, t_dyn_str *res, char *str)
{
	if (!inquote[1] && (*str == '\''))
		inquote[0] = !inquote[0];
	if (!inquote[0] && (*str == '"'))
		inquote[1] = !inquote[1];
	dyn_str_addchar(res, *str);
}

void	handle_ex_var(char **str, int *inquote, t_dyn_str *res)
{
	if (**str == '<' && *(*str + 1) == '<')
	{
		inquote[2] = 1;
		dyn_str_addchar(res, *(*str)++);
		dyn_str_addchar(res, **str);
		(*str)++;
		return ;
	}
	if (**str == '$' && !inquote[0] && !(inquote[2] && inquote[1]))
	{
		handle_dollars(res, str, inquote);
		return ;
	}
	if (**str == '\'' || **str == '"')
		handle_next_quote(inquote, res, *str);
	else
		dyn_str_addchar(res, **str);
	if (inquote[2] && !inquote[0] && !inquote[1]
		&& (ft_isspace(**str) || is_controlchar(**str))
		&& !last_redir(*str))
		inquote[2] = 0;
	(*str)++;
	return ;
}

char	*extract_vars(char *str)
{
	t_dyn_str	*res;
	int			inquote[3];
	char		*var_name;

	res = dyn_str_new();
	inquote[0] = 0;
	inquote[1] = 0;
	inquote[2] = 0;
	while (*str)
		handle_ex_var(&str, inquote, res);
	var_name = res->str;
	return (free(res), var_name);
}
