/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:07:11 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/22 17:50:48 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_redirection_type(t_redirtype type, const char *file)
{
    if (type == REDIR_IN)
        return redirection_in(file);
    else if (type == REDIR_OUT)
        return redirection_out(file);
    else if (type == REDIR_APPEND)
        return redirection_append(file);
    else if (type == REDIR_HEREDOC)
        return apply_heredoc(file);
    return (1);
}

int redir_replace_fd(t_redirection *r)
{
	int targetFD;

	if (r->type == REDIR_OUT)
		targetFD = STDOUT_FILENO;
	else if (r->type == REDIR_APPEND)
		targetFD = STDOUT_FILENO;
	else
		targetFD = STDIN_FILENO;
	if (dup2(r->fd, targetFD) < 0){
		perror("minishell: ");
		close(r->fd);
		return (0);
	}
	close(r->fd);
	return (1);
}

// This should only replace the fd
int apply_redirections(t_command *cmd)
{
    t_redirection *r;
    int result;

    r = cmd->redirections;
    result = 1;
    while (r && !cmd->canceled)
    {
        if (!redir_replace_fd(r))
            result = 0;
        r = r->next;
    }
    return (result);
}

int	redirection_in(const char *file)
{
	int	fd;

	if (!file)
	{
		ft_putstr_fd("minishell: invalid redirection file\n", STDERR_FILENO);
		return (0);
	}
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)file, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		return (0); 
	}
	return (fd); 
}

int	redirection_out(const char *file)
{
	int	fd;

	if (!file)
	{
		ft_putstr_fd("minishell: invalid redirection file\n", STDERR_FILENO);
		return (0);
	}
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)file, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		return (0);
	}
	return (fd);
}

int	redirection_append(const char *file)
{
	int	fd;

	if (!file)
	{
		ft_putstr_fd("minishell: invalid redirection file\n", STDERR_FILENO);
		return (0);
	}
	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)file, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		return (0);
	}
	return (fd);
}