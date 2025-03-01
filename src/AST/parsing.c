/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:59:44 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/01 17:00:15 by ckrasniq         ###   ########.fr       */
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
	// Start by parsing a pipeline
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
	// Check if there's a pipe
	if ((*tokens)->type != TOKEN_PIPE)
		return (left);
	// Consume the pipe token
	*tokens = (*tokens)->next;
	// Parse the right side of the pipe
	right = parse_pipeline(tokens);
	if (!right)
	{
		// Handle error: expected command after pipe
		free_command(left);
		return (NULL);
	}
	// Create a pipeline command
	pipe_cmd = create_pipe_command(left, right);
	return (pipe_cmd);
}


t_command	*parse_simple_command(t_token **tokens)
{
	t_token			*current;
	t_command		*cmd;
	int				arg_count;
	t_redirection	*redir;
	char			*processed_arg;

	current = *tokens;
	cmd = create_simple_command();
	arg_count = 0;
	while (current && current->type != TOKEN_PIPE && current->type != TOKEN_EOF)
	{
		if (is_redirection_token(current->type))
		{
			redir = parse_redirection(&current);
			if (!redir)
				return (free_command(cmd), NULL);
			add_redirection(cmd, redir);
		}
		else if (current->type == TOKEN_WORD)
		{
			processed_arg = handle_quotes(current->value);
			add_argument(cmd, processed_arg);
			free(processed_arg);
			arg_count++;
			current = current->next;
		}
		else
			return (free_command(cmd), NULL);
	}
	*tokens = current;
	if (arg_count == 0)
		return (free_command(cmd), NULL);
	return (cmd);
}

// Parse a redirection
t_redirection	*parse_redirection(t_token **tokens)
{
	t_token			*current;
	t_redirtype		type;
	t_redirection	*redir;
	char			*processed_value;

	current = *tokens;
	if (current->type == TOKEN_REDIRECT_IN)
		type = REDIR_IN;
	else if (current->type == TOKEN_REDIRECT_OUT)
		type = REDIR_OUT;
	else if (current->type == TOKEN_APPEND_OUT)
		type = REDIR_APPEND;
	else if (current->type == TOKEN_HERE_DOCUMENT)
		type = REDIR_HEREDOC;
	else
		return (NULL);
	current = current->next;
	if (!current || current->type != TOKEN_WORD)
		return (NULL);
	processed_value = handle_quotes(current->value);
	redir = create_redirection(type, processed_value);
	free(processed_value);
	// Consume the filename token
	*tokens = current->next;
	return (redir);
}

int	is_redirection_token(t_tokentype type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND_OUT || type == TOKEN_HERE_DOCUMENT);
}