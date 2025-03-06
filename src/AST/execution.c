/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:00:58 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/06 19:05:52 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>
#include "errno.h"

int is_directory(const char *path)
{
	struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        perror("stat failed");
        return 0; // Error, assume not a directory
    }
    return S_ISDIR(path_stat.st_mode);
}

// Execute a simple command
int	execute_simple_command(t_command *cmd, char **env, int fork)
{
	int		saved_fds[3] = {dup(STDIN_FILENO), dup(STDOUT_FILENO),
					dup(STDERR_FILENO)};
	int		ret;
	int		status;
	char	*reppath;

	if (!apply_redirections(cmd))
		return (restore_fds(saved_fds), 1);
	if (is_builtin(cmd->args[0]))
	{
		ret = execute_builtin(cmd, env);
		restore_fds(saved_fds);
		return (ret);
	}
	reppath = find_path(cmd->args[0]);
	if (reppath != NULL)
		cmd->args[0] = reppath;
	if (cmd->canceled)
		return (restore_fds(saved_fds),130);	// shell convention 130 = SIGINT but could be only (1)
	if (fork)
	{
		cmd->pid = ft_fork();
		if (cmd->pid < 0)
			return (perror("fork"), restore_fds(saved_fds), 1);
		if (cmd->pid == 0)
		{
			// check if file is directory, and check if executable
			if (is_directory(cmd->args[0]))
				return (printf(" is a directory"), 126); //replace printf with Error output
			execve(cmd->args[0], cmd->args, get_appstate()->enviroment);
			perror("minishell: ");
			exit(127);
		}
		else
			return (waitpid(cmd->pid, &status, 0), restore_fds(saved_fds), WEXITSTATUS(status));
	}
	else
	{
		//check directory stuff
		execve(cmd->args[0], cmd->args, get_appstate()->enviroment);
		return (EXIT_FAILURE);
	}
}

int	execute_pipe_command(t_command *cmd, char **env)
{
	int	pipefd[2];
	pid_t pid1, pid2;
	int status1, status2;
	
	if (!cmd || !cmd->left || !cmd->right)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
			STDERR_FILENO);
		return (1);
	}
	// Create the pipe
	if (pipe(pipefd) < 0)
		return (perror("minishell: pipe"), 1);
	// Fork for the left side of the pipe (writes to pipe)
	pid1 = ft_fork();
	if (pid1 < 0)
		return (perror("minishell: fork"), close(pipefd[1]), close(pipefd[0]), 1);
	if (cmd->pid == 0)
	{
		// Child process 1: left side of the pipe
		close(pipefd[0]);               // Close read end
		dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
		close(pipefd[1]);
		// Execute the left command
		exit(execute_command(cmd->left, env, 0));
	}
	// Fork for the right side of the pipe (reads from pipe)
	pid2 = ft_fork();
	if (pid2 < 0)
		return (perror("minishell: fork"), close(pipefd[0]), close(pipefd[1]), waitpid(pid1, &status1, 0), 1);
	if (cmd->pid2 == 0)
	{
		// Child process 2: right side of the pipe
		close(pipefd[1]);              // Close write end
		dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe
		close(pipefd[0]);
		// Execute the right command
		exit(execute_command(cmd->left, env, 0));
	}
	// Parent process
	close(pipefd[0]); // Close both ends of the pipe in the parent
	close(pipefd[1]);
	// Wait for both child processes to finish
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	// Return the exit status of the right command
	return (WEXITSTATUS(status2));
}

int	execute_command(t_command *cmd, char **env, int fork)
{
	if (!cmd)
		return (0);
	// expand_variables(cmd, env);
	if (cmd->type == CMD_SIMPLE)
		return (execute_simple_command(cmd, env, fork));
	else if (cmd->type == CMD_PIPE)
		return (execute_pipe_command(cmd, env));
	else
		return (1);
}

/**	Returns a freeable string with the application name in it.
 *	Frees cmd, if path found
 */
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
			free(cmd);
			return (path);
		}
		free(path);
		i++;
	}
	free_string_arr(paths);
	return (NULL);
}

void	free_string_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}
