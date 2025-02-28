/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 14:08:43 by afelger           #+#    #+#             */
/*   Updated: 2025/02/27 19:47:45 by ckrasniq         ###   ########.fr       */
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
	ft_printf("%s: %s: %s\n", progname, msg, target);
	return (1);
}

int	builtin_cd(int argc, char **argv) {
	if (argc > 1)
		display_error(change_directory(argv[1]), argv[0], argv[1]);
	else
		display_error(change_directory(NULL), argv[0], ms_get_env("HOME"));
	return (0);
}
