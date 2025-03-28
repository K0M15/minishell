/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:05:46 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/28 15:08:28 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtin_or_redirections(t_command *cmd, char **env, int *saved_fds)
{
	int	ret;

	if (!apply_redirections(cmd))
		return (restore_fds(saved_fds), 1);
	if (is_builtin(cmd->args[0]))
	{
		ret = execute_builtin(cmd, env);
		restore_fds(saved_fds);
		return (ret);
	}
	return (-1);
}

void	handle_execve_error(char *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd != NULL)
		ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror("");
	exit(127);
}

int	execute_child_process(t_command *cmd)
{
	if (cmd->args[0] && (!is_executable(cmd->args[0])
			|| is_directory(cmd->args[0])))
		return (126);
	execve(cmd->args[0], cmd->args, get_appstate()->enviroment);
	handle_execve_error(cmd->args[0]);
	return (EXIT_FAILURE);
}

int	execute_forked_command(t_command *cmd, int *saved_fds)
{
	int	status;

	cmd->pid = ft_fork();
	if (cmd->pid < 0)
		return (perror("fork"), restore_fds(saved_fds), 1);
	if (cmd->pid == 0)
		execute_child_process(cmd);
	waitpid(cmd->pid, &status, 0);
	restore_fds(saved_fds);
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

char	*find_path(char *cmd)
{
	char	**paths;
	int		i;
	char	*part_path;
	char	*path;

	i = 0;
	paths = ft_split(ms_get_env("PATH"), ':');
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, X_OK) == 0)
		{
			ft_free(cmd);
			free_string_arr(paths);
			return (path);
		}
		ft_free(path);
		i++;
	}
	free_string_arr(paths);
	return (NULL);
}
