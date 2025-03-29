/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_getline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 11:56:30 by afelger           #+#    #+#             */
/*   Updated: 2025/03/29 12:02:12 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
