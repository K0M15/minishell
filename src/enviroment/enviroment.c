/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:05:46 by afelger           #+#    #+#             */
/*   Updated: 2025/02/16 13:30:48 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ms_is_key(char *str, char *key)
{
	int str_c= ft_strlen(key);
	if(ft_strncmp(str, key, str_c == 0) && str[strc_c + 1] == '=')
		return (1);
	return (0);
}

char	*ms_getvalue(char *str)
{
	int c;

	c = 0;
	while(str[c] != '=' && str[c])
		c++;
	c++;
	if (str[c] == 0)
		return (NULL);
	return (&str[c]);
}

char	*ms_getindex(char *key)
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

char	*ms_getvalue(char *key)
{
	char	**enviroment;
	char	*value_curr;
	
	enviroment = get_appstate()->enviroment;
	while (*enviroment)
	{
		
		if (ms_is_key(*enviroment, key))
			return ms_getvalue(*enviroment);
		enviroment++;
	}
	return NULL;
}


char	*ms_set_value(char *key, char *value)
{
	char	**enviroment;
	int		ctr;
	char	*key_curr;
	char	*value_curr;
	
	enviroment = get_appstate()->enviroment;
	while (*enviroment)
	{
		if (ms_is_key(*enviroment, key))
			*enviroment = ft_strjoin(ft_strjoin(key, "="), value);	// THIS COULD ERROR WITH EMPTY VALUE!
		enviroment++;
		ctr++;
	}
}

int	ms_delete_value(char *key)
{
	int pos;

	pos = ms_getindex(key)
	if (pos == -1)
		return (0);
	ms_env_delete(pos);
	return (1);
}


// env data structure

char **ms_env_enhance()
{
	t_appstate *state;
	char **new_env;
	size_t i;

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
	t_appstate *state;

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

	state = get_appstate();
	while(++id < state->env_filled)
		state->enviroment[id - 1] == state->enviroment[id];
	state->enviroment[id - 1] = NULL;
	state->env_filled--;
}
