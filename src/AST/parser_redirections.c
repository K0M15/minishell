/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:02:12 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/22 18:50:42 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int has_quotes(char *str)
{
	while(*str)
	{
		if (*str == '\'' || *str == '"')
			return (1);
		str++;
	}
	return (0);
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
		return (ft_putstr_fd("bash: syntax error near unexpected token `newline'\n" ,2), NULL);
	processed_value = handle_quotes_redir(current->value);	//If heredoc ARG has quotes, Variables arent replaced. Else, they are...
	redir = create_redirection(type, processed_value, has_quotes(current->value));
	free(processed_value);
	*tokens = current->next;
	return (redir);
}

t_redirection	*create_redirection(t_redirtype type, const char *file, int has_quotes)
{
	t_redirection	*redir;
	// insert Redir process here
	// redirections currently have no sense of cmds
	// so redirections cannot be handled good.
	redir = ft_mem_reg(malloc(sizeof(t_redirection)));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_mem_reg(ft_strdup(file));
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->fd = handle_redirection_type(type, file, has_quotes);
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
		// free_command(cmd);
		get_appstate()->last_return = 2;
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
