/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniqi <ckrasniqi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:27:31 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/07 13:22:06 by ckrasniqi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
** Applies a single redirection based on its type.
** @param cmd          The command containing the redirection.
** @param r            The redirection to apply.
** @return             1 on success, 0 if the redirection fails.
*/
int apply_redirection(t_command *cmd, t_redirection *r)
{
    if (r->type == REDIR_IN)
        redirection_in(r);
    else if (r->type == REDIR_OUT)
        redirection_out(r);
    else if (r->type == REDIR_APPEND)
        redirection_append(r);
    else if (r->type == REDIR_HEREDOC)
    {
        if ((cmd->heredoc = ms_heredoc(r->file)) == NULL)
            cmd->canceled = true;
    }
    return (1);
}

/*
** @param cmd          The command to apply redirections to.
** @return             1 on success, 0 if any redirection fails.
*/
int apply_redirections(t_command *cmd)
{
    t_redirection *r = cmd->redirections;

    while (r && !cmd->canceled)
    {
        if (!apply_redirection(cmd, r))
            return (0);
        r = r->next;
    }
    return (1);
}

// int	apply_redirections(t_command *cmd)
// {
// 	t_redirection	*r;
// 	// int				fd[2];

// 	r = cmd->redirections;
// 	while (r && !cmd->canceled)
// 	{
// 		if (r->type == REDIR_IN)
// 			redirection_in(r);
// 		else if (r->type == REDIR_OUT)
// 			redirection_out(r);
// 		else if (r->type == REDIR_APPEND)
// 			redirection_append(r);
// 		else if (r->type == REDIR_HEREDOC)
// 			if((cmd->heredoc = ms_heredoc(r->file)) == NULL)
// 				cmd->canceled = true;
// 		r = r->next;
// 	}
// 	return (1);
// }