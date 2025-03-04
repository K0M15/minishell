/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dyn_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 12:38:13 by afelger           #+#    #+#             */
/*   Updated: 2025/03/04 15:04:23 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dyn_str.h"

t_dyn_str	*dyn_str_new(void)
{
	t_dyn_str	*str;

	str = malloc(sizeof(t_dyn_str));
	if (str->str == NULL)
		return (NULL);
	str->alloc = DYN_STR_ALLOC;
	str->str = malloc(str->alloc);
	if (str->str == NULL)
	{
		free(str);
		return (NULL);
	}
	str->filled = 0;
	str->str[0] = 0;
	return (str);
}

int	dyn_str_enhance(t_dyn_str *str)
{
	char	*temp;

	temp = str->str;
	str->alloc *= 2;
	str->str = malloc(str->alloc);
	if (str->str == NULL)
	{
		str->str = temp;
		return (0);
	}
	ft_memcpy(str->str, temp, str->alloc / 2);
	free(temp);
	return (1);
}

int	dyn_str_addchar(t_dyn_str *str, char c)
{
	if (str->alloc < str->filled + 2)
		if (dyn_str_enhance(str) == 0)
			return (0);
	str->str[str->filled++] = c;
	str->str[str->filled] = 0;
	return (1);
}

int	dyn_str_addstr(t_dyn_str *str, char *src)
{
	size_t	srclen;

	srclen = ft_strlen(src);
	while (str->alloc < str->filled + 1 + srclen)
		if (dyn_str_enhance(str) == 0)
			return (0);
	ft_strlcat(str->str, src, str->alloc);
	str->filled += srclen;
	return (1);
}

void	dyn_str_free(t_dyn_str *str)
{
	free(str->str);
	free(str);
}
