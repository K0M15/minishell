/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:18:28 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/28 15:06:35 by afelger          ###   ########.fr       */
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
			get_appstate()->children = ft_mem_reg(ft_lstnew((void *)pid));
		else
			ft_lstadd_back(&get_appstate()->children,
				ft_mem_reg(ft_lstnew((void *)pid)));
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
		ft_free(arr[i]);
	ft_free(arr);
}
