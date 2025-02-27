/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exampleAST.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:27:31 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/02/27 17:17:14 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (!ptr)
		return (ft_malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = ft_malloc(new_size);
	if (!new_ptr)
		return (NULL);
	copy_size = (old_size < new_size) ? old_size : new_size;
	ft_memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return (new_ptr);
}

// Main parsing function
t_command	*parse(t_token *tokens)
{
	if (!tokens || tokens->type == TOKEN_EOF)
	{
		return (NULL);
	}
	// Start by parsing a pipeline
	return (parse_pipeline(&tokens));
}

// Parse a pipeline (cmd1 | cmd2 | ...)
t_command	*parse_pipeline(t_token **tokens)
{
	t_command	*left;
	t_command	*right;
	t_command	*pipe_cmd;

	// Parse the first command
	left = parse_simple_command(tokens);
	if (!left)
		return (NULL);
	// Check if there's a pipe
	if ((*tokens)->type != TOKEN_PIPE)
		return (left);
	// Consume the pipe token
	*tokens = (*tokens)->next;
	// Parse the right side of the pipe
	right = parse_pipeline(tokens);
	if (!right)
	{
		// Handle error: expected command after pipe
		free_command(left);
		return (NULL);
	}
	// Create a pipeline command
	pipe_cmd = create_pipe_command(left, right);
	return (pipe_cmd);
}

// Parse a simple command (cmd arg1 arg2 ... with possible redirections)
t_command	*parse_simple_command(t_token **tokens)
{
	t_token			*current;
	t_command		*cmd;
	int				arg_count;
	t_redirection	*redir;

	current = *tokens;
	cmd = create_simple_command();
	arg_count = 0;
	// Collect arguments and redirections
	while (current && current->type != TOKEN_PIPE && current->type != TOKEN_EOF)
	{
		if (is_redirection_token(current->type))
		{
			// Handle redirection
			redir = parse_redirection(&current);
			if (!redir)
			{
				// Handle error
				free_command(cmd);
				return (NULL);
			}
			add_redirection(cmd, redir);
		}
		else if (current->type == TOKEN_WORD)
		{
			// Add argument to command
			add_argument(cmd, current->value);
			arg_count++;
			current = current->next;
		}
		else
		{
			// Unexpected token
			free_command(cmd);
			return (NULL);
		}
	}
	// Update tokens to point to the next unprocessed token
	*tokens = current;
	// Return NULL if we didn't find any arguments
	if (arg_count == 0)
	{
		free_command(cmd);
		return (NULL);
	}
	return (cmd);
}

// Parse a redirection
t_redirection	*parse_redirection(t_token **tokens)
{
	t_token			*current;
	t_redirtype		type;
	t_redirection	*redir;

	current = *tokens;
	if (current->type == TOKEN_REDIRECT_IN)
		type = REDIR_IN;
	else if (current->type == TOKEN_REDIRECT_OUT)
		type = REDIR_OUT;
	else if (current->type == TOKEN_APPEND_OUT)
		type = REDIR_APPEND;
	else if (current->type == TOKEN_HERE_DOCUMENT)
		type = REDIR_HEREDOC;
	else
		return (NULL);
	current = current->next;
	if (!current || current->type != TOKEN_WORD)
		return (NULL);
	redir = create_redirection(type, current->value);
	*tokens = current->next;
	return (redir);
}

// Create a simple command
t_command	*create_simple_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->type = CMD_SIMPLE;
	cmd->args = malloc(sizeof(char *) * INITIAL_ARG_CAPACITY);
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->args[0] = NULL;
	cmd->left = NULL;
	cmd->right = NULL;
	cmd->redirections = NULL;
	cmd->pid = 0;
	return (cmd);
}

// Create a pipe command
t_command	*create_pipe_command(t_command *left, t_command *right)
{
	t_command	*cmd;

	cmd = ft_malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->type = CMD_PIPE;
	cmd->args = NULL;
	cmd->left = left;
	cmd->right = right;
	cmd->redirections = NULL;
	return (cmd);
}

// Add an argument to a command
void	add_argument(t_command *cmd, const char *arg)
{
	int		i;
	int		current_capacity;
	int		new_capacity;
	char	**new_args;

	i = 0;
	current_capacity = INITIAL_ARG_CAPACITY;
	// Find the NULL terminator
	while (cmd->args[i] != NULL)
		i++;
	// Check if we need to resize
	if (i >= current_capacity - 1)
	{
		// Resize logic
		new_capacity = current_capacity * 2;
		new_args = ft_realloc(cmd->args, sizeof(char *) * current_capacity, sizeof(char *) * new_capacity);
		if (!new_args)
			return ; // Handle error
		cmd->args = new_args;
		current_capacity = new_capacity;
	}
	// Add the new argument
	cmd->args[i] = ft_strdup(arg);
	cmd->args[i + 1] = NULL;
}

// Create a redirection
t_redirection	*create_redirection(t_redirtype type, const char *file)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	redir->next = NULL;
	return (redir);
}

// Add a redirection to a command
void	add_redirection(t_command *cmd, t_redirection *redir)
{
	t_redirection	*current;

	if (!cmd->redirections)
		cmd->redirections = redir;
	else
	{
		current = cmd->redirections;
		while (current->next)
			current = current->next;
		current->next = redir;
	}
}

// Free a command structure
void	free_command(t_command *cmd)
{
	t_redirection	*r;
	t_redirection	*next;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		for (int i = 0; cmd->args[i]; i++)
			free(cmd->args[i]);
		free(cmd->args);
	}
	r = cmd->redirections;
	while (r)
	{
		next = r->next;
		free(r->file);
		free(r);
		r = next;
	}
	if (cmd->left)
		free_command(cmd->left);
	if (cmd->right)
		free_command(cmd->right);
	free(cmd);
}

// Function to expand variables in a command
void	expand_variables(t_command *cmd, char **env)
{
	t_redirection	*r;

	if (!cmd)
		return ;
	if (cmd->type == CMD_SIMPLE)
		for (int i = 0; cmd->args[i]; i++)
			cmd->args[i] = expand_variables_in_string(cmd->args[i], env);
	r = cmd->redirections;
	while (r)
	{
		if (r->type != REDIR_HEREDOC)
			r->file = expand_variables_in_string(r->file, env);
		r = r->next;
	}
	if (cmd->left)
		expand_variables(cmd->left, env);
	if (cmd->right)
		expand_variables(cmd->right, env);
}

// Helper to expand variables in a string
char	*expand_variables_in_string(const char *str, char **env)
{
	int		length;
	
	(void)env;
	length = ft_strlen(str);
	while(*str)
	{
		if (*str == '$')
		{
			int c = 0;
			while (ft_isspace(str[c]))
				c++;
			char *data = ft_strndup(str, c);
			length = length - c + ft_strlen(ms_get_env(data));
			
		}
		str++;
	}
	return (NULL);
}

// Main execution function
int	execute_command(t_command *cmd, char **env)
{
	if (!cmd)
		return (0);
	expand_variables(cmd, env);
	if (cmd->type == CMD_SIMPLE)
		return (execute_simple_command(cmd, env));
	else if (cmd->type == CMD_PIPE)
		return (execute_pipe_command(cmd, env));
	else
		return (1);
}

int	execute_simple_command(t_command *cmd, char **env)
{
	int		saved_fds[3] = {dup(STDIN_FILENO), dup(STDOUT_FILENO),
				dup(STDERR_FILENO)};
	int		ret;
	int		status;

	if (!apply_redirections(cmd->redirections))
	{
		restore_fds(saved_fds);
		return (1);
	}
	if (is_builtin(cmd->args[0]))
	{
		ret = execute_builtin(cmd, env);
		restore_fds(saved_fds);
		return (ret);
	}
	cmd->pid = fork();
	if (cmd->pid < 0)
	{
		perror("fork");
		restore_fds(saved_fds);
		return (1);
	}
	if (cmd->pid == 0)
	{
		execvp(cmd->args[0], cmd->args);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(cmd->pid, &status, 0);
		restore_fds(saved_fds);
		return (WEXITSTATUS(status));
	}
}

int	execute_pipe_command(t_command *cmd, char **env)
{
	int pipefd[2];
	
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (1);
	}
	cmd->pid = fork();
	if (cmd->pid < 0)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}

	if (cmd->pid == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		execute_command(cmd->left, env);
		exit(EXIT_SUCCESS);
	}

	// Right side of pipe (reads from pipe)
	cmd->pid2 = fork();
	if (cmd->pid2 < 0)
	{
		// KILL First process? no => error broken pipe
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}

	if (cmd->pid2 == 0)
	{
		// Child process 2
		close(pipefd[1]); // Close write end
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);

		execute_command(cmd->right, env);
		exit(EXIT_SUCCESS);
	}

	// Parent process
	close(pipefd[0]);
	close(pipefd[1]);

	int status1, status2;
	waitpid(cmd->pid, &status1, 0);
	waitpid(cmd->pid2, &status2, 0);

	return (WEXITSTATUS(status2)); // Return status of right command
}