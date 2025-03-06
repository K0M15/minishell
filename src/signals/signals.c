/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:03:16 by afelger           #+#    #+#             */
/*   Updated: 2025/03/01 16:47:58 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	{
		rl_done = 1;
		//	stop heredoc function (rl_done = 1 for current readline)
		//	
	}
}

int	ms_sig_kill(t_command *process, int signal)
{
	return (kill(process->pid, signal));
}

int	ms_sig_kill_all(t_list *processes, int signal)
{
	int	buffer;
	
	buffer = 0;
	while(processes != NULL)
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