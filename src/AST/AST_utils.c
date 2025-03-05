/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:56:43 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/05 14:26:46 by afelger          ###   ########.fr       */
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
	t_dyn_str	*result;
	int			in_quotes[2];
	long		ctr;
	char		*tmp;

	result = dyn_str_new();
	ft_memset(in_quotes, 0, 2);
	ctr = 0;
	while (str[ctr])
	{
		if (str[ctr] == '\'' && !in_quotes[1])
			in_quotes[0] = !in_quotes[0];
		else if (str[ctr] == '"' && !in_quotes[0])
			in_quotes[1] = !in_quotes[1];
		else if (str[ctr] == '$' && !in_quotes[0])
		{
			if(add_variable(result, str, &ctr) == 0)
				perror("minishell: ");
		}
		else
			dyn_str_addchar(result, str[ctr]);
		ctr++;
	}
	tmp = result->str;
	return (free(result), tmp);
}
