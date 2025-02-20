/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:59:31 by afelger           #+#    #+#             */
/*   Updated: 2025/02/20 09:16:23 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#define	HISTORY_FILENAME "hist_dump"

int main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	(void) envp;

	ms_env_init();
	ms_display_welcome();
	using_history();
	load_history(HISTORY_FILENAME);
	char *str;
	while (1)
	{
		str = ft_mem_reg(readline(ms_get_prompt()));
		rl_replace_line("something", 2);
		add_history(str); // check beforehand if string is empty
		if (ft_strncmp("exit", str, 4) == 0)
			break;
	}
	dump_history(HISTORY_FILENAME);
	cleanup(0);
}