/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:47:31 by afelger           #+#    #+#             */
/*   Updated: 2025/02/20 09:22:16 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

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
		cleanup(EMEM_NOT_ALLOC);
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
	t_list *mem;
	t_list *last;

	if ((size_t) target > (size_t) &mem)
	{
		ft_printf("tried to free non-freeable"); //target is inside of the stack and not freeable;
		return ;
	}
	last = *get_mem();
	mem = last->next;
	while(mem != NULL && mem->content != NULL){
		last = mem;
		mem = mem->next;
	}
	free(mem->content);
	last->next = mem->next;
	free(mem);
}

void	cleanup(char reason)
{
	t_list	*mem;
	t_list	*last;
	
	mem = (*get_mem())->next;
	while (mem != NULL)
	{
		last = mem;
		mem = mem->next;
		free(last->content);
		free(last);
	}
	free(*get_mem());
	exit(reason);
}
