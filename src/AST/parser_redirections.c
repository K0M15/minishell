/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniqi <ckrasniqi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:02:12 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/09 18:03:41 by ckrasniqi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	*tokens = current->next;
	return (redir);
}

t_redirection	*create_redirection(t_redirtype type, const char *file)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

// Add a redirection to a command
void	add_redirection(t_command *cmd, t_redirection *redir)
{
	t_redirection *current;

	if (!cmd->redirections)
		cmd->redirections = redir;
	else
	{
		current = cmd->redirections;
		while (current->next)
			current = current->next;
		current->next = redir;
	}
}

t_redirection	*handle_redirection_token(t_token **current, t_command *cmd)
{
	t_redirection	*redir;

	redir = parse_redirection(current);
	if (!redir)
	{
		free_command(cmd);
		return (NULL);
	}
	add_redirection(cmd, redir);
	return (redir);
}

int	is_redirection_token(t_tokentype type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND_OUT || type == TOKEN_HERE_DOCUMENT);
}
