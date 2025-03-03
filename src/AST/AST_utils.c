/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:56:43 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/03 14:17:42 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t		ft_fork(void)
{
	long pid;

	pid = fork();
	if (pid < 0)
		return (perror("Fork error"), -1);
	else if (pid == 0)
	{
		ms_sig_init();
		ms_set_state_mode(INTERACTIVE);
		init_child_terminal();
		return pid;
	}
	else
	{
		if (get_appstate()->children == NULL)
			get_appstate()->children = ft_lstnew((void *)pid);
		else
			ft_lstadd_back(&get_appstate()->children, ft_lstnew((void *)pid));
		return pid;
	}
}

char	*ft_strndup(char *dst, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

void	restore_fds(int saved_fds[3])
{
	dup2(saved_fds[0], STDIN_FILENO);
	dup2(saved_fds[1], STDOUT_FILENO);
	dup2(saved_fds[2], STDERR_FILENO);
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (!ptr)
		return (ft_malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = ft_malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (old_size < new_size)
		copy_size = old_size;
	else
		copy_size = new_size;
	ft_memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return (new_ptr);
}


char	*handle_quotes(char *str)
{
	char	*result;
	char	*temp;
	int		in_single_quotes;
	int		in_double_quotes;

	result = ft_strdup("");	//this is not good handling of that. We malloc and free a lot of memory
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (*str)
	{
		if (*str == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (*str == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else
		{
			//hanlde here variable extraction
			temp = result;
			result = ft_strjoin(result, (char[]){*str, '\0'});
			free(temp);
		}
		str++;
	}
	return (result);
}