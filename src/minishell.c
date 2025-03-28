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

char	*extract_vars(char *str)
{
	t_dyn_str *res;
	int offset;
	int inquote[2];
	char *var_name;

	res = dyn_str_new();
	inquote[0] = 0;
	inquote[1] = 0;
	while (*str)
	{
		if (*str == '\'' || *str == '"')
		{// this not working. FIX
			if (!inquote[1] && (*str == '\''))
				inquote[0] = !inquote[0];
			if (!inquote[0] && (*str == '"') )
				inquote[1] = !inquote[1];
			dyn_str_addchar(res, *str);
		}
		else if (*str == '$' && !inquote[0])
		{
			//extract var
			offset = 1;
			while (str[offset] && !ft_isspace(str[offset]) && (ft_isalnum(str[offset]) || str[offset] == '_' || str[offset] == '?'))
				offset++;
			if (offset == 1)
			{
				if (str[1] = '"' || str[1] != '\'') // condition is the wrong way round
					dyn_str_addchar(res, *str);
				str++;
				continue;
			}
			var_name = malloc(offset);
			ft_strndup(var_name, str + 1, offset - 1);
			if (var_name[0] == '?')
			{
				free(var_name);
				var_name = ft_itoa(get_appstate()->last_return);
				dyn_str_addstr(res, var_name);
				offset = 2;
			}
			else
			{
				dyn_str_addstr(res, ms_get_env(var_name));
			}
			free(var_name);
			str += offset - 1;
		}
		else
			dyn_str_addchar(res, *str);
		str++;
	}
	var_name = res->str;
	return (free(res), var_name);
}

void	execute_input(char *str, t_appstate *state)
{
	t_token		*tokens;
	t_command	*cmd;
	int			status;
	char		*buffer;

	buffer = extract_vars(str);
	tokens = tokenize(buffer);
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
	free(buffer);
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
