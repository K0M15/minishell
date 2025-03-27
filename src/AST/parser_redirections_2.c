/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirections_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:11:31 by afelger           #+#    #+#             */
/*   Updated: 2025/03/27 16:17:33 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirection	*handle_redirection_token(t_token **current, t_command *cmd)
{
	t_redirection	*redir;

	redir = parse_redirection(current);
	if (!redir)
	{
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

int	redirection_append(const char *file)
{
	int	fd;

	if (!file)
	{
		ft_putstr_fd("minishell: invalid redirection file\n", STDERR_FILENO);
		return (0);
	}
	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)file, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		get_appstate()->last_return = 1;
		return (-1);
	}
	return (fd);
}
