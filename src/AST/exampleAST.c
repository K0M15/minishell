/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exampleAST.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:27:31 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/01 14:11:25 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t		ft_fork(void)
{
	long pid;

	pid = fork();
	if (pid < 0)
		return (perror("Fork error"), -1);
	else if (pid == 0)
	{
		ms_sig_init();
		ms_set_state_mode(INTERACTIVE);
		init_child_terminal();
		return pid;
	}
	else
	{
		if (get_appstate()->children == NULL)
			get_appstate()->children = ft_lstnew((void *)pid);
		else
			ft_lstadd_back(&get_appstate()->children, ft_lstnew((void *)pid));
		return pid;
	}
}

char	*ft_strndup(char *dst, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

int	is_redirection_token(t_tokentype type)
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
	if (old_size < new_size)
		copy_size = old_size;
	else
		copy_size = new_size;
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

t_command	*parse_pipeline(t_token **tokens)
{
	t_command	*left;
	t_command	*right;
	t_command	*pipe_cmd;

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
char	*handle_quotes(char *str)
{
	char	*result;
	char	*temp;
	int		in_single_quotes;
	int		in_double_quotes;

	result = ft_strdup("");
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (*str)
	{
		if (*str == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (*str == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else
		{
			temp = result;
			result = ft_strjoin(result, (char[]){*str, '\0'});
			free(temp);
		}
		str++;
	}
	return (result);
}

t_command	*parse_simple_command(t_token **tokens)
{
	t_token			*current;
	t_command		*cmd;
	int				arg_count;
	t_redirection	*redir;
	char			*processed_arg;

	current = *tokens;
	cmd = create_simple_command();
	arg_count = 0;
	while (current && current->type != TOKEN_PIPE && current->type != TOKEN_EOF)
	{
		if (is_redirection_token(current->type))
		{
			redir = parse_redirection(&current);
			if (!redir)
				return (free_command(cmd), NULL);
			add_redirection(cmd, redir);
		}
		else if (current->type == TOKEN_WORD)
		{
			processed_arg = handle_quotes(current->value);
			add_argument(cmd, processed_arg);
			free(processed_arg);
			arg_count++;
			current = current->next;
		}
		else
			return (free_command(cmd), NULL);
	}
	*tokens = current;
	if (arg_count == 0)
		return (free_command(cmd), NULL);
	return (cmd);
}

// Parse a redirection
t_redirection	*parse_redirection(t_token **tokens)
{
	t_token			*current;
	t_redirtype		type;
	t_redirection	*redir;
	char			*processed_value;

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
	processed_value = handle_quotes(current->value);
	redir = create_redirection(type, processed_value);
	free(processed_value);
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
		new_args = ft_realloc(cmd->args, sizeof(char *) * new_capacity,
				sizeof(char *) * current_capacity);
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
			cmd->args[i] = expand_variables_in_string(cmd->args[i]);
	r = cmd->redirections;
	while (r)
	{
		if (r->type != REDIR_HEREDOC)
			r->file = expand_variables_in_string(r->file);
		r = r->next;
	}
	if (cmd->left)
		expand_variables(cmd->left, env);
	if (cmd->right)
		expand_variables(cmd->right, env);
}

void	ft_strncat(char *target, const char *source, size_t amount, size_t max)
{
	size_t	counter;
	size_t	max_counter;

	max_counter = 0;
	while (*target && max_counter < max)
	{
		target++;
		max_counter++;
	}
	counter = 0;
	while (source[counter] && counter < amount - 1 && max_counter < max)
	{
		*target = source[counter];
		target++;
		counter++;
		max_counter++;
	}
	*target = '\0';
}

char	*expand_variables_in_string(const char *str)
{
	char	*var_value;
	char	*result;
	char	var_name[256];
	size_t	var_start;

	result = malloc(4 * 1024);
	if (!result)
		return (NULL);
	result[0] = '\0';
	for (size_t i = 0; str[i]; ++i)
	{
		if (str[i] == '$' && str[i + 1] && (str[i + 1] == '_'
				|| ft_isalpha(str[i + 1])))
		{
			i++;
			var_start = i;
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			ft_memcpy(var_name, &str[var_start], i - var_start);
			var_name[i - var_start] = '\0';
			var_value = ms_get_env(var_name);
			ft_strlcat(result, var_value, 4 * 1024);
			free(var_value);
			i--;
		}
		else
			ft_strncat(result, &str[i], 2, 4 * 1024);
	}
	return (result);
}

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
	close(fd);
}

void	redirection_out(t_redirection *redirection)
{
	int	fd;

	fd = open(redirection->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	redirection_append(t_redirection *redirection)
{
	int	fd;

	fd = open(redirection->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

int	apply_redirections(t_redirection *redirections)
{
	t_redirection	*r;
	int				fd[2];

	r = redirections;
	while (r)
	{
		if (r->type == REDIR_IN)
			redirection_in(r);
		else if (r->type == REDIR_OUT)
			redirection_out(r);
		else if (r->type == REDIR_APPEND)
			redirection_append(r);
		else if (r->type == REDIR_HEREDOC)
		{
			if (pipe(fd) < 0)
				return (perror("pipe"), 1);
			if (ms_heredoc(r->file, fd[STDOUT_FILENO]) == -1)
				perror("HEREDOC");
			dup2(fd[STDIN_FILENO], STDIN_FILENO);
			close(fd[STDIN_FILENO]);
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
	int		status;
	char	*reppath;
	
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
	reppath = find_path(cmd->args[0]);
	if (reppath != NULL)
		cmd->args[0] = reppath;
	cmd->pid = ft_fork();
	if (cmd->pid < 0)
	{
		perror("fork");
		restore_fds(saved_fds);
		return (1);
	}
	if (cmd->pid == 0)
	{
		execve(cmd->args[0], cmd->args, get_appstate()->enviroment);
		// execute(cmd->args[0], env);
		perror("execve");
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
	{
		perror("minishell: pipe");
		return (1);
	}
	// Fork for the left side of the pipe (writes to pipe)
	pid1 = ft_fork();
	if (pid1 < 0)
	{
		perror("minishell: fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	if (cmd->pid == 0)
	{
		// Child process 1: left side of the pipe
		close(pipefd[0]);               // Close read end
		dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
		close(pipefd[1]);
		// Execute the left command
		execute_command(cmd->left, env);
		exit(EXIT_SUCCESS);
	}
	// Fork for the right side of the pipe (reads from pipe)
	pid2 = ft_fork();
	if (pid2 < 0)
	{
		perror("minishell: fork");
		close(pipefd[0]);
		close(pipefd[1]);
		waitpid(pid1, &status1, 0); // Wait for the first child to avoid zombies
		return (1);
	}
	if (cmd->pid2 == 0)
	{
		// Child process 2: right side of the pipe
		close(pipefd[1]);              // Close write end
		dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe
		close(pipefd[0]);
		// Execute the right command
		execute_command(cmd->right, env);
		exit(EXIT_SUCCESS);
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

void	free_string_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
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
