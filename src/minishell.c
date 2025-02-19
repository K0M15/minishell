/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:59:31 by afelger           #+#    #+#             */
/*   Updated: 2025/02/19 15:55:18 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

volatile sig_atomic_t g_ms_signal;

void write_hist_entry(int fd, HIST_ENTRY	*entry)
{
	if (entry == NULL)
		return;
	if (write(fd, ) == -1)
		printf("ERR: FD closed");
}

void dump_history()
{
	HISTORY_STATE *state;
	int i;

	state = history_get_history_state();
	i = -1;
	printf("\n");
	while (++i <= state->offset)
		printf("%d: < %ld > %s\n", i, (long) state->entries[i]->timestamp, state->entries[i]->line);
}

int main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	(void) envp;

	ms_env_init();
	ms_display_welcome();
	using_history();
	char *str;
	while (1)
	{
		str = ft_mem_reg(readline(ms_get_prompt()));
		rl_replace_line("something", 2);
		add_history(str); // check beforehand if string is empty
		if (ft_strncmp("exit", str, 4) == 0)
			break;
	}
	dump_history();
	cleanup(0);
}