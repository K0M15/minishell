/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:47:31 by afelger           #+#    #+#             */
/*   Updated: 2025/03/27 15:47:09 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"
#include <stdio.h>

t_list	**get_mem(void)
{
	static t_list	*mem;

	if (mem == NULL)
		mem = ft_lstnew(NULL);
	return (&mem);
}

void	*ft_malloc(size_t size)
{
	void	*mem;

	mem = malloc(size);
	if (mem == NULL)
	{
		perror("minishell: malloc:");
		cleanup(EMEM_NOT_ALLOC);
	}
	ft_lstadd_back(get_mem(), ft_lstnew(mem));
	return (mem);
}

void	*ft_mem_reg(void *mem)
{
	ft_lstadd_back(get_mem(), ft_lstnew(mem));
	return (mem);
}

void	ft_free(void *target)
{
	t_list	*mem;
	t_list	*last;

	last = *get_mem();
	mem = last->next;
	if ((size_t)target > (size_t)(&mem))
		return ;
	while (mem != NULL && mem->content != target)
	{
		last = mem;
		mem = mem->next;
	}
	free(target);
	if (mem != NULL)
	{
		last->next = mem->next;
		free(mem);
	}
}

void	cleanup(char reason)
{
	t_list	*mem;
	t_list	*last;

	mem = (*get_mem());
	while (mem != NULL)
	{
		last = mem;
		mem = mem->next;
		if (last->content != NULL)
			free(last->content);
		free(last);
	}
	exit(reason);
}
