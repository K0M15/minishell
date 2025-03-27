/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   appstate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:05:49 by afelger           #+#    #+#             */
/*   Updated: 2025/03/27 15:32:47 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_appstate	*get_appstate(void)
{
	static t_appstate	state;

	return (&state);
}

void	ms_set_state_mode(t_appmode mode)
{
	if (mode == INTERACTIVE)
		ms_get_sig_action()->sa_sigaction = &ms_sig_handler_interactive;
	else if (mode == HEREDOC)
		ms_get_sig_action()->sa_sigaction = &ms_sig_handler_heredoc;
	else if (mode == RUNNING)
		ms_get_sig_action()->sa_sigaction = &ms_sig_handler_running;
	else
		ft_putstr_fd("\n ERROR: Unknown APPMODE\n", 2);
	get_appstate()->active_mode = mode;
}
