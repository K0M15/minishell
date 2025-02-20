/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:59:31 by afelger           #+#    #+#             */
/*   Updated: 2025/02/20 11:28:37 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	(void) envp;

	ms_env_init();
	ms_display_welcome();
	using_history();
	load_history(HISTORY_FILENAME);
	ms_sig_init();
	init_terminal();
	ms_set_state_mode(INTERACTIVE);
	char *str;
	while (1)
	{
		str = ft_mem_reg(readline(ms_get_prompt()));
		add_history(str); // check beforehand if string is empty
		if (ft_strncmp("exit", str, 4) == 0)
			break;
		if (ft_strncmp("change", str, 6) == 0)
			ms_set_state_mode(RUNNING);
	}
	dump_history(HISTORY_FILENAME);
	cleanup(0);
}