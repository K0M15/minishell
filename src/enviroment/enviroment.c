/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:05:46 by afelger           #+#    #+#             */
/*   Updated: 2025/03/06 18:40:03 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ms_get_env(char *key)
{
	char	**enviroment;

	enviroment = get_appstate()->enviroment;
	while (*enviroment)
	{
		if (ms_is_key(*enviroment, key))
			return (ms_getvalue(*enviroment));
		enviroment++;
	}
	return ("");
}

char	*ms_setvalue(char *key, char *value)
{
	char	**enviroment;
	char	*new_entry;
	char	*buffer;

	buffer = ft_strjoin(key, "=");
	new_entry = ft_strjoin(buffer, value);
	free(buffer);
	enviroment = get_appstate()->enviroment;
	while (*enviroment)
	{
		if (ms_is_key(*enviroment, key))
		{
			free(*enviroment);
			*enviroment = new_entry;
			return (new_entry);
		}
		enviroment++;
	}
	if (ms_env_append(new_entry))
		return (new_entry);
	return (NULL);
}

int	ms_delete_value(char *key)
{
	int	pos;

	pos = ms_getindex(key);
	if (pos == -1)
		return (0);
	ms_env_delete(pos);
	return (1);
}
