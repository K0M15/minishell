/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:05:46 by afelger           #+#    #+#             */
/*   Updated: 2025/03/04 15:14:49 by afelger          ###   ########.fr       */
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
	if (!str[c])
		return ("");
	c++;
	return (&str[c]);
}

int	ms_getindex(char *key)
{
	char	**enviroment;
	int		ctr;

	enviroment = get_appstate()->enviroment;
	ctr = 0;
	while (*enviroment)
	{
		if (ms_is_key(*enviroment, key))
			return (ctr);
		enviroment++;
		ctr++;
	}
	return (-1);
}

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
	new_entry = ft_strjoin(buffer, value); // THIS COULD ERROR WITH EMPTY VALUE!
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
		return new_entry;
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

// env data structure

char	**ms_env_enhance()
{
	t_appstate	*state;
	char		**new_env;
	size_t		i;

	state = get_appstate();
	new_env = malloc(state->env_alloc * 2);
	state->env_alloc *= 2;
	i = 0;
	ft_memcpy(state->enviroment, new_env, state->env_filled);
	free(state->enviroment);
	state->enviroment = new_env;
	return (new_env);
}

int	ms_env_append(char *str)
{
	t_appstate	*state;

	state = get_appstate();
	if (state->env_alloc == state->env_filled + 2)
		if (ms_env_enhance() == NULL)
			return (0);
	state->enviroment[state->env_filled] = ft_strdup(str);
	state->enviroment[state->env_filled + 1] = NULL;
	state->env_filled++;
	return (1);
}

void	ms_env_delete(int id)
{
	t_appstate	*state;
	size_t		ctr;

	state = get_appstate();
	if (id < 0)
		return ;
	ctr = (unsigned long) id;
	free(state->enviroment[ctr]);
	while(++ctr < state->env_filled)
		state->enviroment[ctr - 1] = state->enviroment[ctr];
	state->enviroment[ctr - 1] = NULL;
	state->env_filled--;
}

int	ms_env_init(void)
{
	int c;
	t_appstate *state;

	c = 0;
	state = get_appstate();
	state->enviroment = malloc(ENV_ALLOC_SIZE);
	if (state->enviroment == NULL)
		return (0);
	state->env_alloc = ENV_ALLOC_SIZE;
	state->env_filled = 0;
	while (environ[c])
	{
		ms_env_append(environ[c]);
		c++;
	}
	return (1);
}
