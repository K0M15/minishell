/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:18:28 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/18 17:43:07 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


pid_t	ft_fork(void)
{
	long	pid;

	pid = fork();
	if (pid < 0)
		return (perror("Fork error"), -1);
	else if (pid == 0)
	{
		ms_sig_init();
		ms_set_state_mode(INTERACTIVE);
		init_child_terminal();
		return (pid);
	}
	else
	{
		if (get_appstate()->children == NULL)
			get_appstate()->children = ft_lstnew((void *)pid);
		else
			ft_lstadd_back(&get_appstate()->children, ft_lstnew((void *)pid));
		return (pid);
	}
}

void	toggle_quote(int *quote_flag)
{
	*quote_flag = !(*quote_flag);
}

void	free_string_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		perror("stat failed");
		return (0); // Error, assume not a directory
	}
	return (S_ISDIR(path_stat.st_mode));
}

void	process_char(t_dyn_str *result, char *str, long *ctr, int *in_quotes)
{
	if (str[*ctr] == '$' && !in_quotes[0])
	{
		if ((str[*ctr+1] == '\'' && !in_quotes[0]) || (str[*ctr+1] == '"'
			&& !in_quotes[1]))
			return ;
		if (add_variable(result, str, ctr) == 0)
			perror("minishell: ");
	}
	else
		dyn_str_addchar(result, str[*ctr]);
}