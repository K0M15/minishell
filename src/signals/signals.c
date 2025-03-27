/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:03:16 by afelger           #+#    #+#             */
/*   Updated: 2025/03/27 15:36:17 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_ms_signal;

t_sigaction	*ms_get_sig_action(void)
{
	static t_sigaction	sa;

	return (&sa);
}

void	ms_sig_handler_interactive(int signal, siginfo_t *info, void *ctx)
{
	if (signal == SIGQUIT)
		return ;
	(void) info;
	(void) ctx;
	g_ms_signal = signal;
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ms_sig_handler_heredoc(int signal, siginfo_t *info, void *ctx)
{
	(void) info;
	(void) ctx;
	if (signal == SIGINT)
		rl_done = 1;
}

int	ms_sig_kill(t_command *process, int signal)
{
	return (kill(process->pid, signal));
}
