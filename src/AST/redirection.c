/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:27:31 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/03 12:56:16 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int	apply_redirections(t_command *cmd)
{
	t_redirection	*r;
	// int				fd[2];

	r = cmd->redirections;
	while (r && !cmd->canceled)
	{
		if (r->type == REDIR_IN)
			redirection_in(r);
		else if (r->type == REDIR_OUT)
			redirection_out(r);
		else if (r->type == REDIR_APPEND)
			redirection_append(r);
		else if (r->type == REDIR_HEREDOC)
			if((cmd->heredoc = ms_heredoc(r->file)) == NULL)
				cmd->canceled = true;
		r = r->next;
	}
	return (1);
}