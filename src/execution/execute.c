/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:38:54 by afelger           #+#    #+#             */
/*   Updated: 2025/02/24 17:39:41 by afelger          ###   ########.fr       */
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
			//	SHELL PROCESS
			waitpid(cmdone->pid, &stat_loc[0], 0);
			waitpid(cmdone->pid, &stat_loc[1], 0);
			cmdone->ret_value = WEXITSTATUS(stat_loc[0]);
			cmdtwo->ret_value = WEXITSTATUS(stat_loc[1]);
			return (1);
		}
		// cmdtwo process
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		close(pipefd[1]);
		if (execve(cmdtwo->prg_name, cmdtwo->argv, get_appstate()->enviroment) == -1)
			exit(-1);
	}
	// cmdone process
	dup2(pipefd[1], 1);
	close(pipefd[0]);
	close(pipefd[1]);
	if (execve(cmdone->prg_name, cmdone->argv, get_appstate()->enviroment) == -1)
		exit(-1);
	return (1);
}

int run_piped(t_command *cmds, int amount)
{
	int pipefd[3];
	int *stat_loc;
	int	counter;

	counter = 0;
	stat_loc = malloc(amount);
	while(counter < amount)
	{
		if (pipe(pipefd) != 0)	// [0] == read, [1] == write
			return (-1);
		if (counter != (amount - 1)){
			cmds[counter + 1].pipe_in = pipefd[0];
			cmds[counter].pipe_out = pipefd[1];
		}
		cmds[counter].pid = fork();
		if (cmds[counter].pid < -1)
			return (-1);				// Pipe broken, SIGINT all other progs
		else if(cmds[counter].pid != 0)
		{	//0 and amount -1 get read / write not duplicated OR 0 and 1 should be already set
			dup2(cmds[counter].pipe_in, 0);
			dup2(cmds[counter].pipe_out, 1);
			if (execve(cmds[counter].prg_name, cmds[counter].argv, get_appstate()->enviroment) == -10)
				exit(-1);
		}
		else{
			if (close(pipefd[0]) < 0 || close(pipefd[1] < 0))
				return (-1);
			counter++;
		}
	}
	counter = -1;
	while(++counter < amount)
	{
		waitpid(cmds[counter].pid, stat_loc[counter], 0);
		cmds[counter].ret_value = WEXITSTATUS(stat_loc[counter]);
	}
	free(stat_loc);
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