/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_checkers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:43:31 by afelger           #+#    #+#             */
/*   Updated: 2025/03/29 15:16:36 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/stat.h>

int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (0);
	if (S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putstr_fd(": is a directory\n", 2);
		return (126);
	}
	return (0);
}

int	is_executable(const char *path)
{
	int	result;

	result = access(path, X_OK);
	if (result != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (0);
	}
	return (1);
}

char	*exists(char *path)
{
	char	*res;

	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
			return (path);
	}
	if (path[0] == '.' || path[0] == '/')
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (NULL);
	}
	res = find_path(path);
	if (res != NULL)
		return (res);
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd("\n", 2);
	return (NULL);
}

int	is_builtin(char *str)
{
	if (str != NULL && (ft_strlencmp(str, "echo") == 0
			|| ft_strlencmp(str, "cd") == 0
			|| ft_strlencmp(str, "env") == 0
			|| ft_strlencmp(str, "exit") == 0
			|| ft_strlencmp(str, "export") == 0
			|| ft_strlencmp(str, "unset") == 0
			|| ft_strlencmp(str, "pwd") == 0))
		return (1);
	return (0);
}

int	execute_builtin(t_command *cmd, char **env)
{
	int	argc;

	argc = count_args(cmd->args);
	(void)env;
	if (ft_strlencmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(argc, cmd->args));
	else if (ft_strlencmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(argc, cmd->args));
	else if (ft_strlencmp(cmd->args[0], "env") == 0)
		return (builtin_env(argc, cmd->args));
	else if (ft_strlencmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(argc, cmd->args));
	else if (ft_strlencmp(cmd->args[0], "export") == 0)
		return (builtin_export(argc, cmd->args));
	else if (ft_strlencmp(cmd->args[0], "unset") == 0)
		return (builtin_unset(argc, cmd->args));
	else if (ft_strlencmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd(argc, cmd->args));
	return (0xFF);
}
