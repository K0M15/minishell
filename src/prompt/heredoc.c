/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:38:08 by afelger           #+#    #+#             */
/*   Updated: 2025/03/28 15:53:39 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	apply_heredoc(const char *dellimter, int no_variable_exp)
{
	int	fd;

	fd = ms_heredoc(dellimter, no_variable_exp);
	if (fd < 0)
		ft_putstr_fd("minishell: heredoc failed\n", STDERR_FILENO);
	return (fd);
}

void	ms_doc_free(t_doc *document)
{
	t_doc	*next;

	while (document != NULL)
	{
		next = document->next;
		free(document->content);
		free(document);
		document = next;
	}
}

char	*get_heredoc_line(void)
{
	char	*str;
	char	*line;

	if (isatty(STDIN_FILENO))
		str = ft_mem_reg(readline("> "));
	else
	{
		line = get_next_line(fileno(stdin));
		if (line != NULL)
			str = ft_strtrim(line, "\n");
		else
			str = NULL;
		free(line);
	}
	return (str);
}

static int	heredoc_process(const char *dellimter,
	t_doc **document, int no_var_exp)
{
	char	*str;
	t_doc	*last;
	int		iseof;

	str = get_heredoc_line();
	if (str == NULL)
		return (0);
	if (!no_var_exp)
		str = expand_variables_in_string(str);
	iseof = (ft_strncmp(str, dellimter, ft_strlen(dellimter)) == 0
			&& ft_strlen(str) == ft_strlen(dellimter));
	if (iseof)
		return (ft_free(str), 0);
	last = ms_doc_app_or_new(document);
	if (last == NULL || get_appstate()->cancled_heredoc)
		return (ft_free(str), -1);
	last->content = ft_strjoin(str, "\n");
	last->length = ft_strlen(last->content);
	iseof = heredoc_process(dellimter, &last, no_var_exp);
	if (iseof == -1 || get_appstate()->cancled_heredoc)
		return (ft_free(str), -1);
	if (*document != NULL)
		return (ft_free(str), iseof);
	*document = last;
	return (ft_free(str), iseof);
}

static int	split_writer(t_doc *document, int no_var_exp)
{
	int	pid;
	int	pipedoc[2];

	if (pipe(pipedoc) == -1)
		return (-1);
	pid = ft_fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		close(pipedoc[STDIN_FILENO]);
		ms_doc_display_free(document, pipedoc[STDOUT_FILENO], no_var_exp);
		close(pipedoc[STDOUT_FILENO]);
		exit(0);
	}
	close(pipedoc[STDOUT_FILENO]);
	return (pipedoc[STDIN_FILENO]);
}

int	ms_heredoc(const char *delimiter, int no_var_exp)
{
	t_doc	*document;

	document = NULL;
	ms_set_state_mode(HEREDOC);
	if (heredoc_process(delimiter, &document, no_var_exp) < 0)
		return (-1);
	return (split_writer(document, no_var_exp));
}
