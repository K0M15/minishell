/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:38:08 by afelger           #+#    #+#             */
/*   Updated: 2025/02/26 19:16:45 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int ms_heredoc(char *dellimter, int fd, t_doc *document)
{
	char			*str;
	t_doc	*last;
	int				iseof;

	str = readline("> "); 
	iseof = (ft_strncmp(str, dellimter, ft_strlen(dellimter)) == 0
		&& ft_strlen(str) == dellimter);
	if (iseof)
		return (0);
	last = ms_doc_app_or_new(&document);
	if (last == NULL)
		return (-1);
	last->content = ft_strjoin(str, "\n");
	free(str);
	last->length = ft_strlen(last->content);
	iseof = ms_heredoc(dellimter, fd, last);
	if (iseof == -1)
		return (-1);
	if (document != NULL)
		return (iseof);
	ms_doc_display_free(last, fd);
	return (iseof);
}
