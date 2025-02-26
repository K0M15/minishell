/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:59:31 by afelger           #+#    #+#             */
/*   Updated: 2025/02/26 16:58:42 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

// int main(int argc, char **argv, char **envp)
// {
// 	(void) argc;
// 	(void) argv;
// 	(void) envp;

// 	ms_env_init();
// 	ms_display_welcome();
// 	using_history();
// 	load_history(HISTORY_FILENAME);
// 	ms_sig_init();
// 	init_terminal();
// 	ms_set_state_mode(INTERACTIVE);
// 	char *str;
// 	while (1)
// 	{
// 		str = ft_mem_reg(readline(ms_get_prompt()));
// 		add_history(str); // check beforehand if string is empty
// 		if (ft_strncmp("exit", str, 4) == 0)
// 			break;
// 		if (ft_strncmp("change", str, 6) == 0)
// 			ms_set_state_mode(RUNNING);
// 	}
// 	dump_history(HISTORY_FILENAME);
// 	cleanup(0);

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
// 	{
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

int	main(void)
{
	char *input;
	Token *tokens;
	Token *current;

	while (1)
	{
		input = readline("minishell: ");
		if (!input) // Handle Ctrl+D
			break ;
		tokens = tokenize(input);
		current = tokens;
		while (current)
		{
			printf("Type: ");
			print_token_type(current->type);
			if (current->value)
				printf(", Value: %s \n", current->value);
			else
				printf("\n");
			current = current->next;
		}
		free_tokens(tokens);
		free(input);
	}
	return (0);
}