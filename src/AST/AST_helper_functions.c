/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST_helper_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:58:08 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/03 12:54:25 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	cmd->heredoc = NULL;
	cmd->canceled = false;
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
