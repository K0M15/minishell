/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniqi <ckrasniqi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 17:58:31 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/09 17:59:57 by ckrasniqi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Main parsing function
t_command	*parse(t_token *tokens)
{
	if (!tokens || tokens->type == TOKEN_EOF)
	{
		return (NULL);
	}
	return (parse_pipeline(&tokens));
}
t_command	*parse_pipeline(t_token **tokens)
{
	t_command	*left;
	t_command	*right;
	t_command	*pipe_cmd;

	left = parse_simple_command(tokens);
	if (!left)
		return (NULL);
	if ((*tokens)->type != TOKEN_PIPE)
		return (left);
	*tokens = (*tokens)->next;
	right = parse_pipeline(tokens);
	if (!right)
	{
		free_command(left);
		return (NULL);
	}
	pipe_cmd = create_pipe_command(left, right);
	return (pipe_cmd);
}

t_command	*initialize_simple_command(void)
{
	return (create_simple_command());
}

t_command	*parse_simple_command(t_token **tokens)
{
	t_command	*cmd;
	int			arg_count;
	t_token		*current;

	cmd = initialize_simple_command();
	current = *tokens;
	arg_count = process_command_tokens(&current, cmd);
	if (arg_count == -1 || arg_count == 0)
		return (free_command(cmd), NULL);
	*tokens = current;
	return (cmd);
}

int	process_command_tokens(t_token **current, t_command *cmd)
{
	int	arg_count;

	arg_count = 0;
	while (*current && (*current)->type != TOKEN_PIPE
		&& (*current)->type != TOKEN_EOF)
	{
		if (is_redirection_token((*current)->type))
		{
			if (!handle_redirection_token(current, cmd))
				return (-1);
		}
		else if ((*current)->type == TOKEN_WORD)
		{
			if (!handle_word_token(current, cmd))
				return (-1);
			arg_count++;
		}
		else
			return (-1);
	}
	return (arg_count);
}
