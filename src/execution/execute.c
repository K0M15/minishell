/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:38:54 by afelger           #+#    #+#             */
/*   Updated: 2025/02/22 17:38:00 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

// char	*get_prg_from_var(char *prg_name, char **env)
// {
// 	//need to read string splited by :
// 	//search 
// }

char	*find_prg_path(char *prg_name)
{
	if (prg_name[0] == '.' && prg_name[0] == '/')
		return (prg_name);
	else
		ft_printf("PATH not implemented");
		return (prg_name);
		// return (get_prg_from_var(prg_name), ms_get_env("PATH"))
}

int run_single_pipe(t_command *cmdone, t_command *cmdtwo)
{
	int pipefd[2];
	int stat_loc[2];

	if (pipe(pipefd) != 0)
		return (-1);
	cmdone->pid = fork();
	if (cmdone->pid == -1)
		return (-1);
	else if (cmdone->pid != 0)
	{
		cmdtwo->pid = fork();
		if (cmdtwo->pid == -1)
			return (-1); // kill first fork? YES
		else if (cmdtwo->pid != 0)
		{
			waitpid(cmdone->pid, &stat_loc[0], 0);
			waitpid(cmdone->pid, &stat_loc[1], 0);
			cmdone->ret_value = WEXITSTATUS(stat_loc[0]);
			cmdtwo->ret_value = WEXITSTATUS(stat_loc[1]);
			return 1;
		}
		// cmdtwoprocess
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		close(pipefd[1]);
		if (execve(cmdtwo->prg_name, cmdtwo->argv, get_appstate()->enviroment) == -1)
			exit(-1);
	}
	dup2(pipefd[1], 1);
	close(pipefd[0]);
	close(pipefd[1]);
	if (execve(cmdone->prg_name, cmdone->argv, get_appstate()->enviroment) == -1)
		exit(-1);
	printf("NOT IMPLEMENTED"); // not implemented
	return (1);
}

int run_piped(t_command *cmds, int amount)
{
	(void) cmds;
	(void) amount;
	printf("NOT IMPLEMENTED"); // not implemented
	return (1);
}

int run_command(t_command	*cmd)
{
	// char	*prg_path;
	int stat_loc;
	cmd->pid = fork();
	if (cmd->pid == -1)
		return (-1);
	else if (cmd->pid == 0)
	{
		if (execve(cmd->prg_name, cmd->argv, get_appstate()->enviroment) == -1)
			exit(-1);
	}
	waitpid(cmd->pid, &stat_loc, 0);
	if (WIFEXITED(stat_loc))
		cmd->ret_value = WEXITSTATUS(stat_loc);
	else
		cmd->ret_value = -1;
	return (0);
}