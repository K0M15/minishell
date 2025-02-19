/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:59:31 by afelger           #+#    #+#             */
/*   Updated: 2025/02/18 15:27:24 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

volatile sig_atomic_t g_ms_signal;

int main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	(void) envp;

	ms_env_init();
	ms_display_welcome();
	char *str;
	while (1)
	{
		str = readline(ms_get_prompt());
		free(str);
	}
}