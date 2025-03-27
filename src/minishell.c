/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:59:31 by afelger           #+#    #+#             */
/*   Updated: 2025/03/27 18:36:11 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	anakin(void)
{
	t_list	*next;
	t_list	*children;
	int		status;

	children = get_appstate()->children;
	while (children != NULL)
	{
		next = children->next;
		kill((long) children->content, SIGKILL);
		waitpid((long) children->content, &status, 0);
		ft_free (children);
		children = next;
	}
	get_appstate()->children = NULL;
	return (1);
}

t_appstate	*init(void)
{
	t_appstate	*state;

	state = get_appstate();
	ms_env_init();
	if (isatty(STDIN_FILENO))
	{
		ms_display_welcome();
		state->appname = "minishell";
	}
	else
		state->appname = "bash";
	ms_sig_init();
	init_terminal();
	ms_set_state_mode(INTERACTIVE);
	return (get_appstate());
}

char	*get_line(void)
{
	char	*str;
	char	*line;

	if (isatty(STDIN_FILENO))
		str = ft_mem_reg(readline(ms_get_prompt()));
	else
	{
		line = get_next_line(fileno(stdin));
		if (line != NULL)
			str = ft_strtrim(line, "\n");
		else
			str = NULL;
		free(line);
	}
	if (str == NULL)
	{
		if (isatty(STDIN_FILENO))
			printf("exit\n");
	}
	return (str);
}

void	execute_input(char *str, t_appstate *state)
{
	t_token		*tokens;
	t_command	*cmd;
	int			status;

	tokens = tokenize(str);
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
	add_history(str);
}

int	main(void)
{
	t_appstate	*state;
	char		*str;

	state = init();
	while (!state->stop)
	{
		str = get_line();
		if (str == NULL)
			break ;
		execute_input(str, state);
	}
	cleanup(get_appstate()->last_return);
}
