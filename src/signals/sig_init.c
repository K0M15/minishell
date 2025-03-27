/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 15:33:36 by afelger           #+#    #+#             */
/*   Updated: 2025/03/27 15:37:32 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_sig_init(void)
{
	t_sigaction	*sa;

	sa = ms_get_sig_action();
	sa->sa_sigaction = &ms_sig_handler_interactive;
	sa->sa_flags = SA_SIGINFO;
	sigemptyset(&sa->sa_mask);
	if (sigaction(SIGINT, sa, NULL) == -1 || sigaction(SIGQUIT, sa, NULL))
	{
		ft_putstr_fd("Problem while setting up signals", 2);
	}
}

int	ms_sig_kill_all(t_list *processes, int signal)
{
	int	buffer;

	buffer = 0;
	while (processes != NULL)
	{
		buffer = buffer & ms_sig_kill((t_command *)processes->content, signal);
	}
	return (buffer);
}

void	ms_sig_handler_running(int signal, siginfo_t *info, void *ctx)
{
	t_appstate	*state;

	(void) info;
	(void) ctx;
	g_ms_signal = signal;
	state = get_appstate();
	printf("Recieved SIGNAL");
	ft_putstr_fd("^C\n", 1);
	rl_on_new_line();
	ms_sig_kill_all(state->children, signal);
	ms_set_state_mode(INTERACTIVE);
}
