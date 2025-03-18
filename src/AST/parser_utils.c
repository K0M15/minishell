/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:00:10 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/18 13:30:14 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	cmd->args_cap = INITIAL_ARG_CAPACITY;
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

// Add an argument to a command
void	add_argument(t_command *cmd, const char *arg)
{
	size_t	i;
	size_t	new_capacity;
	char	**new_args;

	i = 0;
	while (cmd->args[i] != NULL)
		i++;
	if (i >= cmd->args_cap - 1)
	{
		new_capacity = cmd->args_cap * 2;
		new_args = ft_realloc(cmd->args, sizeof(char *) * cmd->args_cap,
				sizeof(char *) * new_capacity);
		if (!new_args)
			return ;
		cmd->args = new_args;
		cmd->args_cap = new_capacity;
	}
	cmd->args[i] = ft_strdup(arg);
	cmd->args[i + 1] = NULL;
}

int	handle_word_token(t_token **current, t_command *cmd)
{
	char	*processed_arg;

	processed_arg = handle_quotes((*current)->value);
	if (!processed_arg)
		return (0);
	add_argument(cmd, processed_arg);
	free(processed_arg);
	*current = (*current)->next;
	return (1);
}
