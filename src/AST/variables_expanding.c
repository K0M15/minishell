/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_expanding.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:03:12 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/06 12:54:54 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_variables(t_command *cmd, char **env)
{
	t_redirection	*r;

	if (!cmd)
		return ;
	if (cmd->type == CMD_SIMPLE)
		for (int i = 0; cmd->args[i]; i++)
			cmd->args[i] = expand_variables_in_string(cmd->args[i]);
	r = cmd->redirections;
	while (r)
	{
		if (r->type != REDIR_HEREDOC)
			r->file = expand_variables_in_string(r->file);
		r = r->next;
	}
	if (cmd->left)
		expand_variables(cmd->left, env);
	if (cmd->right)
		expand_variables(cmd->right, env);
}

int	add_variable(t_dyn_str *result, char *str, long *pos)
{
	char	*var_name;
	char	*var_value;
	int		ctr;

	if (str[*pos] == '$' && str[*pos + 1] == '?')
	{
		var_value = ft_itoa(get_appstate()->last_return);
		dyn_str_addstr(result, var_value);
		(*pos)++;
		return (free(var_value), 1);
	}
	if ((str[*pos] != '$' || !(ft_isalpha(str[*pos + 1]) || str[*pos + 1] == '_'))\
		&& *pos - 1 >= 0 && !ft_isalpha((str[*pos - 1])))
		return (1);
	if ((str[*pos] != '$' || !(ft_isalpha(str[*pos + 1]) || str[*pos + 1] == '_' || str[*pos + 1] == '"')) \
		&& *pos - 1 >= 0 && ft_isalnum((str[*pos-1]))) // i feel to dumb for that
		return (dyn_str_addchar(result, '$'),1);
	ctr = *pos + 1;
	while(str[ctr] && (ft_isalnum(str[ctr]) || str[ctr] == '_') && str[ctr] != '\'' && str[ctr] != '"')
		ctr++;
	var_name = malloc(ctr - *pos);
	ft_strndup(var_name, &(str[*pos + 1]), ctr - *pos - 1);
	var_value = ms_get_env(var_name);
	dyn_str_addstr(result, var_value);
	*pos += ctr - *pos - 1;
	return (free (var_name), 1);
}

char	*expand_variables_in_string(const char *str)
{
	char	*var_value;
	char	*result;
	char	var_name[256];
	size_t	var_start;

	result = malloc(4 * 1024);
	if (!result)
		return (NULL);
	result[0] = '\0';
	for (size_t i = 0; str[i]; ++i)
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] == '?')
		{
			var_value = ft_itoa(get_appstate()->last_return);
			ft_strlcat(result, var_value, 4 * 1024);
			i += 1;
			free(var_value);
		}
		else if (str[i] == '$' && str[i + 1] && (str[i + 1] == '_'
				|| ft_isalpha(str[i + 1])))
		{
			i++;
			var_start = i;
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			ft_memcpy(var_name, &str[var_start], i - var_start);
			var_name[i - var_start] = '\0';
			var_value = ms_get_env(var_name);
			ft_strlcat(result, var_value, 4 * 1024);
			// free(var_value);
			i--;
		}
		else
			ft_strncat(result, &str[i], 2, 4 * 1024);
	}
	return (result);
}

void	ft_strncat(char *target, const char *source, size_t amount, size_t max)
{
	size_t	counter;
	size_t	max_counter;

	max_counter = 0;
	while (*target && max_counter < max)
	{
		target++;
		max_counter++;
	}
	counter = 0;
	while (source[counter] && counter < amount - 1 && max_counter < max)
	{
		*target = source[counter];
		target++;
		counter++;
		max_counter++;
	}
	*target = '\0';
}