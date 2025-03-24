/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:03:50 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/24 14:58:28 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_command *cmd, char **env, int fork)
{
	if (!cmd)
		return (0);
	if (cmd->type == CMD_SIMPLE)
 		return (execute_simple_command(cmd, env, fork));
	else if (cmd->type == CMD_PIPE)
		return (execute_pipe_command(cmd, env));
	else
		return (1);
}

static int handle_dots(t_command *cmd)
{
	if (ft_strlencmp(cmd->args[0], ".") == 0)
	{
		ft_putstr_fd("bash: .: filename argument required\n", 2);
		ft_putstr_fd(".: usage: . filename [arguments]\n", 2);
		return (2);
	}
	else if (ft_strlencmp(cmd->args[0], "..") == 0)
	{
		ft_putstr_fd("bash: ..: command not found\n", 2);
		return (127);
	}
	return (0);
}

int	execute_simple_command(t_command *cmd, char **env, int fork)
{
	int		saved_fds[3];
	int		ret;

	if (save_fds(saved_fds))
		return (perror("dup"), 1);
	ret = handle_builtin_or_redirections(cmd, env, saved_fds);
	if (ret != -1)
		return (restore_fds(saved_fds), ret);
	if (cmd->args[0] != NULL)
	{	
		ret = handle_dots(cmd);
		if (ret)
			return (ret);
		if (ft_strlencmp(cmd->args[0], "") == 0)
			return (0);
		cmd->args[0] = exists(cmd->args[0]);
		if (is_directory(cmd->args[0]))
			return (126);
		if (cmd->args[0] == NULL)
			return (127);
		if (cmd->canceled)
			return (restore_fds(saved_fds), 130);
		if (fork)
			return (execute_forked_command(cmd, saved_fds));
		return (execute_child_process(cmd));
	}
	else if (cmd->args[0] == NULL)
		return (127);
	return (0);
}

int	execute_pipe_command(t_command *cmd, char **env)
{
	pid_t	pid1;
	pid_t	pid2;

	int pipefd[2], status1, status2;
	if (!cmd || !cmd->left || !cmd->right)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
			STDERR_FILENO);
		return (1);
	}
	if (create_pipe(pipefd))
		return (1);
	pid1 = fork_and_execute_left(cmd, env, pipefd);
	pid2 = fork_and_execute_right(cmd, env, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFSIGNALED(status1))
		return (128 + WTERMSIG(status1));
	if (WIFSIGNALED(status2))
		return (128 + WTERMSIG(status2));
	return (WEXITSTATUS(status2));
}

int	execute_builtin(t_command *cmd, char **env)
{
	int	argc;

	argc = count_args(cmd->args);
	(void)env;
	if (ft_strlencmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(argc, cmd->args));
	else if (ft_strlencmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(argc, cmd->args));
	else if (ft_strlencmp(cmd->args[0], "env") == 0)
		return (builtin_env(argc, cmd->args));
	else if (ft_strlencmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(argc, cmd->args));
	else if (ft_strlencmp(cmd->args[0], "export") == 0)
		return (builtin_export(argc, cmd->args));
	else if (ft_strlencmp(cmd->args[0], "unset") == 0)
		return (builtin_unset(argc, cmd->args));
	else if (ft_strlencmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd(argc, cmd->args));
	return (0xFF);
}

int	is_builtin(char *str)
{	// Create cmp without cases 
	if (str != NULL && (ft_strlencmp(str, "echo") == 0
		|| ft_strlencmp(str, "cd") == 0 || ft_strlencmp(str, "env") == 0
		|| ft_strlencmp(str, "exit") == 0 || ft_strlencmp(str, "export") == 0
		|| ft_strlencmp(str, "unset") == 0 || ft_strlencmp(str, "pwd") == 0))
		return (1);
	return (0);
}
