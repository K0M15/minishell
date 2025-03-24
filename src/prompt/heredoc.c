/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:38:08 by afelger           #+#    #+#             */
/*   Updated: 2025/03/24 14:22:47 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	apply_heredoc(const char *dellimter, int no_variable_exp)
{
	int fd;
	
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

static int	heredoc_process(const char *dellimter, t_doc **document, int no_var_exp)
{
	char	*str;
	t_doc	*last;
	int		iseof;

	str = readline("> ");
	if (str == NULL)
		return (0);
	if (!no_var_exp)
		str = expand_variables_in_string(str);	//mem leak
	iseof = (ft_strncmp(str, dellimter, ft_strlen(dellimter)) == 0
			&& ft_strlen(str) == ft_strlen(dellimter));
	if (iseof)
		return (free(str), 0);
	last = ms_doc_app_or_new(document);
	if (last == NULL || get_appstate()->cancled_heredoc)
		return (free(str), -1);
	last->content = ft_strjoin(str, "\n");
	last->length = ft_strlen(last->content);
	iseof = heredoc_process(dellimter, &last, no_var_exp);
	if (iseof == -1 || get_appstate()->cancled_heredoc)
		return (free(str), -1);
	if (*document != NULL)
		return (free(str), iseof);
	*document = last;
	return (free(str), iseof);
}

static int split_writer(t_doc *document, int no_var_exp)
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
	t_doc *document;

	document = NULL;
	ms_set_state_mode(HEREDOC);
	if (heredoc_process(delimiter, &document, no_var_exp) <  0)
		return (-1);
	return (split_writer(document, no_var_exp));
}

// int	ms_heredoc(char *delimiter, int fd, t_doc *document)
// {
// 	char	*line;
// 	int		is_eof;

// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 			break ;
// 		is_eof = (ft_strcmp(line, delimiter) == 0);
// 		if (is_eof)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		write(fd, line, ft_strlen(line));
// 		write(fd, "\n", 1);
// 		free(line);
// 	}
// 	return (0);
// }
