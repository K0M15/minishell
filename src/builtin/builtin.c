/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:36:14 by afelger           #+#    #+#             */
/*   Updated: 2025/02/27 20:55:09 by ckrasniq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(const unsigned char *)s1 - *(const unsigned char *)s2);
}

int	is_builtin(char *str)
{
	if (ft_strcmp(str, "echo") == 0 || ft_strcmp(str, "cd") == 0
		|| ft_strcmp(str, "env") == 0 || ft_strcmp(str, "exit") == 0
		|| ft_strcmp(str, "export") == 0 || ft_strcmp(str, "unset") == 0
		|| ft_strcmp(str, "pwd") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_command *cmd, char **env)
{
	int	argc;

	argc = count_args(cmd->args);
	(void)env;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(argc, cmd->args));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(argc, cmd->args));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (builtin_env(argc, cmd->args));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(argc, cmd->args));
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (builtin_export(argc, cmd->args));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (builtin_unset(argc, cmd->args));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd(argc, cmd->args));
	return (0xFF);
}
