/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_strfunc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 18:34:36 by afelger           #+#    #+#             */
/*   Updated: 2025/03/07 17:15:50 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_is_key(char *str, char *key)
{
	int	str_c;

	str_c = ft_strlen(key);
	if (ft_strncmp(str, key, str_c) == 0 && str[str_c] == '=')
		return (1);
	return (0);
}

char	*ms_getkey(char *str)
{
	int		c;
	char	*result;

	c = 0;
	while (str[c] && str[c] != '=')
		c++;
	result = malloc(++c);
	if (result == NULL)
		return (NULL);
	result[--c] = '\0';
	while (c > 0)
	{
		c--;
		result[c] = str[c];
	}
	return (result);
}

char	*ms_getvalue(char *str)
{
	int	c;

	c = 0;
	while (str[c] && str[c] != '=')
		c++;
	if (!str[c] && c > 0 && str[c] == '=')
		return ("");
	if (!str[c] && c > 0 && str[c] != '=')
		return NULL;
	c++;
	return (&str[c]);
}
