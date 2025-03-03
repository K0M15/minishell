/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:38:08 by afelger           #+#    #+#             */
/*   Updated: 2025/03/01 16:48:08 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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

int	heredoc_process(char *dellimter, t_doc **document)
{
	char	*str;
	t_doc	*last;
	int		iseof;

	str = readline("> ");
	if (str == NULL)
		return (0);
	iseof = (ft_strncmp(str, dellimter, ft_strlen(dellimter)) == 0
			&& ft_strlen(str) == ft_strlen(dellimter));
	if (iseof)
		return (0);
	last = ms_doc_app_or_new(document);
	if (last == NULL || get_appstate()->cancled_heredoc)
		return (-1);
	last->content = ft_strjoin(str, "\n");
	free(str);
	last->length = ft_strlen(last->content);
	iseof = heredoc_process(dellimter, &last);
	if (iseof == -1 || get_appstate()->cancled_heredoc)
		return (-1);
	if (document != NULL)
		return (iseof);
	*document = last;
	return (iseof);
}

t_doc	*ms_heredoc(char *delimiter)
{
	t_doc *document;

	document = NULL;
	ms_set_state_mode(HEREDOC);
	if (heredoc_process(delimiter, &document) <  0)
		return (NULL);
	return (document);
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
