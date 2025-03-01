/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:07:19 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/01 17:08:16 by ckrasniq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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