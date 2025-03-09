/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniqi <ckrasniqi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 17:52:47 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/09 17:54:09 by ckrasniqi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_strncat(char *target, const char *source, size_t amount, size_t max)
{
	size_t	counter;
	size_t	max_counter;

	max_counter = 0;
	while (*target && max_counter < max)
	{
		target++;
		max_counter++;
	}
	counter = 0;
	while (source[counter] && counter < amount - 1 && max_counter < max)
	{
		*target = source[counter];
		target++;
		counter++;
		max_counter++;
	}
	*target = '\0';
}

char	*ft_strcpy(char *dst, const char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
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

