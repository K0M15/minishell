/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 09:00:55 by afelger           #+#    #+#             */
/*   Updated: 2025/03/27 15:41:15 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "fcntl.h"
#include "get_next_line.h"

int	write_hist_entry(int fd, HIST_ENTRY *entry)
{
	if (entry == NULL)
		return (-1);
	ft_putstr_fd(entry->line, fd);
	ft_putchar_fd('\n', fd);
	return (0);
}

void	ft_replace(char *content, char search, char replace, int truncate)
{
	int	len;

	len = ft_strlen(content) - 1;
	while (len + 1 && content[len])
	{
		if (truncate && content[len] != search)
			truncate = 0;
		else if (content[len] == search && truncate)
			content[len] = 0;
		else if (content[len] == search)
			content[len] = replace;
		len--;
	}
}

void	load_history(char *filename)
{
	char	*content;
	int		fd;

	fd = open(filename, O_RDONLY, NULL);
	if (fd == -1)
		return ;
	content = ft_mem_reg(get_next_line(fd));
	while (content != NULL)
	{
		ft_replace(content, '\n', ' ', 1);
		add_history(content);
		content = ft_mem_reg(get_next_line(fd));
	}
}

int	dump_history(char *filename)
{
	HISTORY_STATE	*state;
	int				fd;
	int				i;

	state = history_get_history_state();
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	i = 0;
	while (++i <= state->offset)
	{
		if (write_hist_entry(fd, state->entries[i]) == -1)
			return (-1);
		ft_free(state->entries[i]);
	}
	if (close(fd) == -1)
		return (-1);
	return (0);
}
