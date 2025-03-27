/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_expanding.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:03:12 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/27 16:30:49 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_variables(t_command *cmd, char **env)
{
	t_redirection	*r;

	if (!cmd)
		return ;
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
		return (handle_exit_status(result, pos));
	if (str[*pos + 1] == '\0' || (!ft_isalpha(str[*pos + 1]) && str[*pos
				+ 1] != '_' && str[*pos + 1] != '?'))
	{
		dyn_str_addchar(result, '$');
		return (1);
	}
	ctr = *pos + 1;
	while (str[ctr] && (ft_isalnum(str[ctr]) || str[ctr] == '_'))
		ctr++;
	var_name = malloc(ctr - *pos);
	ft_strndup(var_name, &(str[*pos + 1]), ctr - *pos - 1);
	var_value = ms_get_env(var_name);
	if (var_value)
		dyn_str_addstr(result, var_value);
	*pos += ctr - *pos - 1;
	free(var_name);
	return (1);
}

void	handle_env_var(const char *str, char *result, size_t *i)
{
	char	var_name[256];
	size_t	var_start;
	char	*var_value;

	(*i)++;
	var_start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	ft_memcpy(var_name, &str[var_start], *i - var_start);
	var_name[*i - var_start] = '\0';
	var_value = ms_get_env(var_name);
	ft_strlcat(result, var_value, 4 * 1024);
	(*i)--;
}

char	*expand_variables_in_string(const char *str)
{
	char	*result;
	size_t	i;

	result = malloc(4 * 1024);
	if (!result)
		return (NULL);
	result[0] = '\0';
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] == '?')
			handle_exit_status_var(result, &i);
		else if (str[i] == '$' && str[i + 1] && (str[i + 1] == '_'
				|| ft_isalpha(str[i + 1])))
			handle_env_var(str, result, &i);
		else
			ft_strncat(result, &str[i], 2, 4 * 1024);
		i++;
	}
	return (result);
}
