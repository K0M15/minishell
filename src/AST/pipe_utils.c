/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:11:41 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/27 16:16:47 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	save_fds(int *saved_fds)
{
	saved_fds[0] = dup(STDIN_FILENO);
	saved_fds[1] = dup(STDOUT_FILENO);
	saved_fds[2] = dup(STDERR_FILENO);
	return (saved_fds[0] == -1 || saved_fds[1] == -1 || saved_fds[2] == -1);
}

int	create_pipe(int *pipefd)
{
	if (pipe(pipefd) < 0)
		return (perror("minishell: pipe"), 1);
	return (0);
}

pid_t	fork_and_execute_left(t_command *cmd, char **env, int *pipefd)
{
	pid_t	pid;

	pid = ft_fork();
	if (pid < 0)
		return (perror("minishell: fork"), -1);
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exit(execute_command(cmd->left, env, 0));
	}
	return (pid);
}

pid_t	fork_and_execute_right(t_command *cmd, char **env, int *pipefd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("minishell: fork"), -1);
	if (pid == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		cleanup(execute_command(cmd->right, env, 0));
	}
	return (pid);
}

void	restore_fds(int saved_fds[3])
{
	dup2(saved_fds[0], STDIN_FILENO);
	dup2(saved_fds[1], STDOUT_FILENO);
	dup2(saved_fds[2], STDERR_FILENO);
	close(saved_fds[0]);
	close(saved_fds[1]);
	close(saved_fds[2]);
}
