/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 14:08:43 by afelger           #+#    #+#             */
/*   Updated: 2025/03/23 12:09:24 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "errno.h"

static char	*handle_minus()
{
	char	*oldpwd;

	oldpwd = ms_get_env("OLDPWD");
	if (ft_strlen(oldpwd) == 0)
	{
		write(2, "minishell: cd: OLDPWD not set\n", 30);
		return (NULL);
	}
	ft_putstr_fd(oldpwd, 1);
	ft_putchar_fd('\n', 1);
	return (oldpwd);
}

static int	change_directory(const char *path)
{
	int		chresult;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (path == NULL)
		path = ms_get_env("HOME");
	if (path == NULL)
		return (write(2, "minishell: cd: HOME not set\n", 28),free(cwd), 1);
	else if (ft_strlencmp("-", path) == 0)
		path = handle_minus();
	if (path == NULL)
		return (free(cwd), 1);
	chresult = chdir(path);
	if (chresult == 0)
	{
		if (ms_setvalue("OLDPWD", cwd) == NULL)
			perror("minishell: cd: ");
		free(cwd);
		cwd = getcwd(NULL, 0);
		if (ms_setvalue("PWD", cwd) == NULL)
			perror("minishell: cd: ");
	}
	return (free(cwd), chresult);
}

static int	display_error(int errcode, char *target)
{
	char	*buff;

	if (errcode && errno)
	{
		buff = ft_strjoin("bash: cd: ", target);
		perror(buff);
		free(buff);
		return (1);
	}
	else if (errcode)
		return (1);
	return (0);
}

int	builtin_cd(int argc, char **argv)
{
	if (argc > 1)
		return (display_error(change_directory(argv[1]), argv[1]));
	return (display_error(change_directory(NULL), ms_get_env("HOME")));
}
