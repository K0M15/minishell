/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:59:31 by afelger           #+#    #+#             */
/*   Updated: 2025/02/26 16:32:47 by ckrasniq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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