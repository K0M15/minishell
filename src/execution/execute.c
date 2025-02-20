/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:38:54 by afelger           #+#    #+#             */
/*   Updated: 2025/02/20 11:59:53 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

char	*find_prg_path(char *prg_name);
int	add_prg_to_argc(char *prg_name, char **args)
{
	int c;

	c = 0;
	while(argc[c] != NULL && c < ARG_MAX)
	{

		c++;
	}
}

int run_command(t_command	*cmd)
{
	char	*prg_path;
	// find program in path?
	prg_path = find_prg_path(cmd->prg_name)
	if (prg_path == -1)
		return (-1);
	if(add_prg_to_argc(cmd->prg_name, cmd->args) != 1)
		return (-1);
	cmd->pid = execve(prg_path, cmd->args, get_appstate()->enviroment)
}