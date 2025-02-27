/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exampleAST.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:27:31 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/02/27 17:24:57 by ckrasniq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redeirection_token(TokenType type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND_OUT || type == TOKEN_HERE_DOCUMENT);
}

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
t_command	*parse(Token *tokens)
{
	if (!tokens || tokens->type == TOKEN_EOF)
	{
		return (NULL);
	}
	// Start by parsing a pipeline
	return (parse_pipeline(tokens));
}

// Parse a pipeline (cmd1 | cmd2 | ...)
t_command	*parse_pipeline(Token **tokens)
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
t_command	*parse_simple_command(Token **tokens)
{
	Token			*current;
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
t_redirection	*parse_redirection(Token **tokens)
{
	Token			*current;
	RedirType		type;
	t_redirection	*redir;

	current = *tokens;
	// Map token types to redirection types using if-else structure
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
	// Consume the redirection token
	current = current->next;
	// Check if there's a file/delimiter
	if (!current || current->type != TOKEN_WORD)
		// Handle error: expected filename after redirection
		return (NULL);
	// Create redirection
	redir = create_redirection(type, current->value);
	// Consume the filename token
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
	cmd->args[0] = NULL; // Null-terminate initially
	cmd->left = NULL;
	cmd->right = NULL;
	cmd->redirections = NULL;
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
		new_args = ft_realloc(cmd->args, sizeof(char *) * new_capacity);
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
t_redirection	*create_redirection(RedirType type, const char *file)
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
	{
		cmd->redirections = redir;
	}
	else
	{
		// Add to the end of the list
		current = cmd->redirections;
		while (current->next)
		{
			current = current->next;
		}
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
	// Free arguments
	if (cmd->args)
	{
		for (int i = 0; cmd->args[i]; i++)
		{
			free(cmd->args[i]);
		}
		free(cmd->args);
	}
	// Free redirections
	r = cmd->redirections;
	while (r)
	{
		next = r->next;
		free(r->file);
		free(r);
		r = next;
	}
	// Free child commands
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
	{
		// Expand variables in arguments
		for (int i = 0; cmd->args[i]; i++)
		{
			cmd->args[i] = expand_variables_in_string(cmd->args[i], env);
		}
	}
	// Expand variables in redirections
	r = cmd->redirections;
	while (r)
	{
		if (r->type != REDIR_HEREDOC)
		{ // Usually not expanded in heredocs
			r->file = expand_variables_in_string(r->file, env);
		}
		r = r->next;
	}
	// Recurse for pipe commands
	if (cmd->left)
		expand_variables(cmd->left, env);
	if (cmd->right)
		expand_variables(cmd->right, env);
}

// Helper to expand variables in a string
char	*expand_variables_in_string(const char *str, char **env)
{
	char *var_value;
    char *result = malloc(4*1024); // Allocate buffer (adjust size as needed)
    char var_name[256];

    if (!result)
		return NULL;
    result[0] = '\0'; // Initialize empty string

    for (size_t i = 0; str[i]; i++) {
        if (str[i] == '$' && str[i + 1] && (str[i + 1] == '_'
			|| ft_isalpha(str[i + 1]))) {
            i++;
            size_t var_start = i;
            while (str[i] && (ft_isalnum(str[i]) || str[i] == '_')) i++;

            ft_strndup(var_name, &str[var_start], i - var_start);
            var_name[i - var_start] = '\0';

            var_value = ms_get_env(var_name);
            ft_strlcat(result, var_value, 4*1024);
            free(var_value);
            i--;
        } else {
            ft_strlcat(result, &str[i], 2);
        }
    }
    return (result);
}
	// Implementation depends on your shell's behavior
	// Should handle:
	// 1. Regular variables: $HOME, $USER, etc.
	// 2. Quoted variables: "$HOME", '$USER', etc.
	// 3. Special cases: $?, $$, etc.
	// This is a complex function - would need specific implementation


// Main execution function
int	execute_command(t_command *cmd, char **env)
{
	if (!cmd)
		return (0);
	// Expand variables before execution
	expand_variables(cmd, env);
	// Execute based on command type
	if (cmd->type == CMD_SIMPLE)
		return (execute_simple_command(cmd, env));
	else if (cmd->type == CMD_PIPE)
		return (execute_pipe_command(cmd, env));
	else
		return (1); // Error - unknown command type
}

void	restore_fds(int saved_fds[3])
{
	dup2(saved_fds[0], STDIN_FILENO);
	dup2(saved_fds[1], STDOUT_FILENO);
	dup2(saved_fds[2], STDERR_FILENO);
}

void	redirection_in(t_redirection *redirection)
{
	int	fd;

	fd = open(redirection->file, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
}

void	redirection_out(t_redirection *redirection)
{
	int	fd;

	fd = open(redirection->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open");
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
}

void	redirection_append(t_redirection *redirection)
{
	int fd;

	fd = open(redirection->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
}

int	apply_redirections(t_redirection *redirections)
{
	t_redirection	*r;
	int				fd[2];

	r = redirections;
	while (r)
	{
		if (r->type == REDIR_IN)
		{
			redirection_in(r);
		}
		else if (r->type == REDIR_OUT)
		{
			redirection_out(r);
		}
		else if (r->type == REDIR_APPEND)
		{
			redirection_append(r);
		}
		else if (r->type == REDIR_HEREDOC)
		{
			if (pipe(fd) < 0)
			{
				perror("pipe");
				return (1);
			}
			ms_heredoc(redirections->file, fd[STDIN_FILENO], NULL);
		}
		r = r->next;
	}
	return (1);
}

// Execute a simple command
int	execute_simple_command(t_command *cmd, char **env)
{
	int		saved_fds[3] = {dup(STDIN_FILENO), dup(STDOUT_FILENO),
				dup(STDERR_FILENO)};
	int		ret;
	pid_t	pid;
	int		status;

	// Apply redirections
	if (!apply_redirections(cmd->redirections))
	{
		// Restore file descriptors on error
		restore_fds(saved_fds);
		return (1);
	}
	// Check for built-in commands
	if (is_builtin(cmd->args[0]))
	{
		ret = execute_builtin(cmd, env);
		restore_fds(saved_fds);
		return (ret);
	}
	// Fork and execute external command
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		restore_fds(saved_fds);
		return (1);
	}
	if (pid == 0)
	{
		// Child process
		execvp(cmd->args[0], cmd->args);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else
	{
		// Parent process
		waitpid(pid, &status, 0);
		restore_fds(saved_fds);
		return (WEXITSTATUS(status));
	}
}

// Execute a pipe command
int	execute_pipe_command(t_command *cmd, char **env)
{
	int pipefd[2];
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (1);
	}

	// Left side of pipe (writes to pipe)
	pid_t pid1 = fork();
	if (pid1 < 0)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}

	if (pid1 == 0)
	{
		// Child process 1
		close(pipefd[0]); // Close read end
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		execute_command(cmd->left, env);
		exit(EXIT_SUCCESS);
	}

	// Right side of pipe (reads from pipe)
	pid_t pid2 = fork();
	if (pid2 < 0)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}

	if (pid2 == 0)
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
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);

	return (WEXITSTATUS(status2)); // Return status of right command
}