/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:38:08 by afelger           #+#    #+#             */
/*   Updated: 2025/03/01 11:32:05 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_process(char *dellimter, int fd, t_doc *document)
{
	char	*str;
	t_doc	*last;
	int		iseof;

	str = readline("> ");
	iseof = (ft_strncmp(str, dellimter, ft_strlen(dellimter)) == 0
			&& ft_strlen(str) == ft_strlen(dellimter));
	if (iseof)
		return (0);
	last = ms_doc_app_or_new(&document);
	if (last == NULL)
		return (-1);
	last->content = ft_strjoin(str, "\n");
	free(str);
	last->length = ft_strlen(last->content);
	iseof = heredoc_process(dellimter, fd, last);
	if (iseof == -1)
		return (-1);
	if (document != NULL)
		return (iseof);
	ms_doc_display_free(last, fd);
	return (iseof);
}

int ms_heredoc(char *delimiter, int fd) {
    pid_t pid;
	
	pid = fork();
    if (pid == 0)
	{
		ms_set_state_mode(HEREDOC);
        heredoc_process(delimiter, fd, NULL);
        exit(0);
    }
	else if (pid > 0)
        return (close(fd), 0);
    return (perror("fork failed"), -1);
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
