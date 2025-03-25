/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_checkers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:43:31 by afelger           #+#    #+#             */
/*   Updated: 2025/03/25 17:46:30 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/stat.h>

int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (0); // Error, assume not a directory
	if (S_ISDIR(path_stat.st_mode))
	{
		write(2, "minishell: ", 11);
		write(2, path, ft_strlen(path));
		write(2, ": is a directory\n", 17);
		return (126);
	}
	return (0);
}

int is_executable(const char *path)
{
	int result;
	
	result = access(path, X_OK);
	if (result != 0)
	{
		write(2, "minishell: ", 11);
		write(2, path, ft_strlen(path));
		write(2, ": Permission denied\n", 20);
		return (0);
	}
	return (1);
}

char *exists(char *path)
{
	char *res;

	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
			return (path);
	}
	if(path[0] == '.')
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
