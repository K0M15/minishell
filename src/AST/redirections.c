/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniqi <ckrasniqi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:07:11 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/09 18:10:06 by ckrasniqi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_redirection_type(t_command *cmd, t_redirection *r)
{
    if (r->type == REDIR_IN)
        return redirection_in(r);
    else if (r->type == REDIR_OUT)
        return redirection_out(r);
    else if (r->type == REDIR_APPEND)
        return redirection_append(r);
    else if (r->type == REDIR_HEREDOC)
        return apply_heredoc(cmd, r);
    return (1);
}

int apply_redirections(t_command *cmd)
{
    t_redirection *r;
    int result;

    r = cmd->redirections;
    result = 1;
    
    while (r && !cmd->canceled)
    {
        if (!handle_redirection_type(cmd, r))
            result = 0;
        r = r->next;
    }
    
    return (result);
}

int	redirection_in(t_redirection *redirection)
{
	int	fd;

	if (!redirection->file)
	{
		ft_putstr_fd("minishell: invalid redirection file\n", STDERR_FILENO);
		return (0);
	}
	fd = open(redirection->file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(redirection->file, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		return (0); 
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("minishell: dup2");
		close(fd);
		return (0); 
	}
	close(fd);
	return (1); 
}

int	redirection_out(t_redirection *redirection)
{
	int	fd;

	if (!redirection->file)
	{
		ft_putstr_fd("minishell: invalid redirection file\n", STDERR_FILENO);
		return (0);
	}
	fd = open(redirection->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(redirection->file, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("minishell: dup2");
		close(fd);
		return (0);
	}
	close(fd);
	return (1); 
}

int	redirection_append(t_redirection *redirection)
{
	int	fd;

	if (!redirection->file)
	{
		ft_putstr_fd("minishell: invalid redirection file\n", STDERR_FILENO);
		return (0);
	}
	fd = open(redirection->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(redirection->file, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("minishell: dup2");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}