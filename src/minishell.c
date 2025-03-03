/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:59:31 by afelger           #+#    #+#             */
/*   Updated: 2025/03/01 17:08:48 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_ctrl_d(void)
{
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	cleanup(0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	char		*input;
// 	Token		*tokens;
// 	t_command	*cmd;
// 	int			status;

// 	(void)argc;
// 	(void)argv;
// 	ms_env_init();
// 	ms_sig_init();
// 	init_terminal();
// 	while (1)
// 	{
// 		input = readline("minishell> ");
// 		if (!input) // Handle Ctrl+D
// 		{
// 			handle_ctrl_d();
// 			continue ;
// 		}
// 		if (ft_strlen(input) > 0)
// 			add_history(input);
// 		tokens = tokenize(input);
// 		cmd = parse(tokens);
// 		if (cmd)
// 		{
// 			status = execute_command(cmd, envp);
// 			free_command(cmd);
// 		}
// 		free_tokens(tokens);
// 		free(input);
// 	}
// 	return (0);
// }

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
	// ms_display_welcome();
	using_history();
	load_history(HISTORY_FILENAME);
	ms_sig_init();
	init_terminal();
	ms_set_state_mode(INTERACTIVE);
	char *str;
	while (1)
	{
		str = ft_mem_reg(readline(ms_get_prompt()));
		if (str == NULL)
		{
			printf("exit");	//go back to last line and write "exit"
			cleanup(0);
		}
		tokens = tokenize(str);
		cmd = parse(tokens);
		if (cmd)
		{
			status = execute_command(cmd, state->enviroment);
			state->last_return = status;
			free_command(cmd);
			init_terminal();
		}
		free_tokens(tokens);
		// free(str);
		add_history(str); // check beforehand if string is emptys
		if (ft_strncmp("change", str, 6) == 0)
			ms_set_state_mode(RUNNING);
	}
	dump_history(HISTORY_FILENAME);
	cleanup(0);
	return (0);
}

// int	main(void)
// {
// 	char		*input;
// 	Token		*tokens;
// 	// t_command	*cmd;
// 	int			status;
// 	extern char	**environ;

// 	status = 0;
// 	// Get environment
// 	while (1)
// {
// 		input = readline("minishell: ");
// 		if (!input)
// 		{
// 			printf("\nExiting shell\n");
// 			break ;
// 		}
// 		add_history(input);
// 		tokens = tokenize(input);
// 		// cmd = parse(tokens);
// 		// if (cmd)
// 		// {
// 		// 	status = execute_command(cmd, environ);
// 		// 	free_command(cmd);
// 		// }
// 		free_tokens(tokens);
// 		free(input);
// 	}
// 	return (status);
// }

// int	main(void)
// {
// 	char *input;
// 	Token *tokens;
// 	Token *current;
// 	ms_display_welcome();
// 	while (1)
// 	{
// 		input = readline(ms_get_prompt());
// 		if (!input) // Handle Ctrl+D
// 			break ;
// 		tokens = tokenize(input);
// 		current = tokens;
// 		while (current)
// 		{
// 			printf("Type: ");
// 			print_token_type(current->type);
// 			if (current->value)
// 				printf(", Value: %s \n", current->value);
// 			else
// 				printf("\n");
// 			current = current->next;
// 		}
// 		free_tokens(tokens);
// 		free(input);
// 	}
// 	return (0);
// }
