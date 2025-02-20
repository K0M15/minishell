/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:03:16 by afelger           #+#    #+#             */
/*   Updated: 2025/02/20 09:28:16 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct sigaction	*get_sig_action(void)
{
	static struct sigaction	sa;

	return (&sa);
}

void	ms_sig_handler(int signal, siginfo_t *info, void *ctx)
{
	(void) info;
	(void) ctx;
	g_ms_signal = signal;
}

void	ms_sig_init(void)
{
	struct sigaction	*sa;

	sa = get_sig_action();
	sa->sa_sigaction = &ms_sig_handler;
	sa->sa_flags = 0;					//read this again.. are there usefull flags?
	sigemptyset(&sa->sa_mask);
	sigaction(SIGINT, sa, NULL);
}