/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 14:08:43 by afelger           #+#    #+#             */
/*   Updated: 2025/03/03 13:59:22 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "errno.h"

static int	change_directory(const char *path) {
	int	chresult;

	if (path == NULL)
	{
		path = ms_get_env("HOME");
	}
	chresult = chdir(path);
	return (chresult);
}

static int	display_error(int errcode, char* progname, char *target)
{
	char	*msg;

	msg = "";
	if (errcode == 0)
	{
		return (0);
	}
	else if (errno == EACCES)
		msg = "Permission denied";
	else if (errno == ELOOP)
		msg = "Too many symbolic links";
	else if (errno == ENAMETOOLONG)
		msg = "File or path name too long";
	else if (errno == ENOENT)
		msg = "Too many symbolic links";
	else if (errno == ENOTDIR)
		msg = "Not a directory";
	write(STDERR_FILENO, "cd: ", 4);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	return (1);
}

int	builtin_cd(int argc, char **argv) {
	if (argc > 1)
		display_error(change_directory(argv[1]), argv[0], argv[1]);
	else
		// Check if target is folder or file
		// bash: line 1: cd: test: Not a directory
		// bash: line 1: cd: 1123123: No such file or directory
		display_error(change_directory(NULL), argv[0], ms_get_env("HOME"));
	return (0);
}
