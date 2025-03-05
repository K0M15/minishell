/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 14:08:43 by afelger           #+#    #+#             */
/*   Updated: 2025/03/05 16:14:18 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "errno.h"

static int	change_directory(const char *path) {
	int	chresult;
	char *cwd;

	cwd = getcwd(NULL, 0);
	if (path == NULL)
		path = ms_get_env("HOME");
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
	char *buff;

	if (errcode)
	{
		buff = ft_strjoin("bash: cd: ", target);
		perror(buff);
		free(buff);
		return (1);
	}
	return (0);
}

int	builtin_cd(int argc, char **argv) {
	if (argc > 1)
		return (display_error(change_directory(argv[1]), argv[1]));
	// Check if target is folder or file
	// bash: line 1: cd: test: Not a directory
	// bash: line 1: cd: 1123123: No such file or directory
	return (display_error(change_directory(NULL), ms_get_env("HOME")));
}
