/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:56:43 by ckrasniq          #+#    #+#             */
/*   Updated: 2025/03/21 15:33:30 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (!ptr)
		return (ft_malloc(new_size));
	if (new_size == 0)
	{
		ft_free(ptr);
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
	ft_free(ptr);
	return (new_ptr);
}

int	handle_exit_status(t_dyn_str *result, long *pos)
{
	char	*var_value;

	var_value = ft_itoa(get_appstate()->last_return);
	dyn_str_addstr(result, var_value);
	(*pos)++;
	free(var_value);
	return (1);
}
