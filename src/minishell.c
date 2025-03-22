/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:59:31 by afelger           #+#    #+#             */
/*   Updated: 2025/03/22 19:57:56 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int anakin()
{
	t_list *next;
	t_list *current;
	int status;

	current = get_appstate()->children;
	while (current != NULL)
	{
		next = current->next;
		kill((long) current->content, SIGKILL);
		waitpid((long) current->content, &status, 0);
		ft_free (current);
		current = next;
	}
	get_appstate()->children = NULL;
	return (1);
}


int main (int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	(void) envp;
	t_token		*tokens;
	t_command	*cmd;
	t_appstate			*state;
	int status;

	state = get_appstate();
	ms_env_init();
	if(isatty(STDIN_FILENO))
	{
		ms_display_welcome();
		state->appname = argv[0];
	}
	else
		state->appname = "bash";
	using_history();
	ms_sig_init();
	init_terminal();
	ms_set_state_mode(INTERACTIVE);
	char *str;
	while (!state->stop)
	{
		if(isatty(STDIN_FILENO))
			str = ft_mem_reg(readline(ms_get_prompt()));
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			if (line != NULL)
				str = ft_strtrim(line, "\n");
			else
				str = NULL;
			free(line);
		}
		if (str == NULL)
		{
			if(isatty(STDIN_FILENO))
				printf("exit");	//go back to last line and write "exit"
			break ;
		}
		tokens = tokenize(str);	// Variable expansion must take place in tokenize to find tokens in variables... but then we got the problem with HEREDOCS in quotes
		cmd = parse(tokens);
		if (cmd)
		{
			status = execute_command(cmd, state->enviroment, 1);
			state->last_return = status;
			free_command(cmd);
			anakin();
			init_terminal();
		}
		free_tokens(tokens);
		// free(str);
		add_history(str); // check beforehand if string is emptys
		if (ft_strncmp("change", str, 6) == 0)
			ms_set_state_mode(RUNNING);
	}
	cleanup(get_appstate()->last_return);
}