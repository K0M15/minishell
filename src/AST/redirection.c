/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:27:31 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/01 17:12:11 by ckrasniq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



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