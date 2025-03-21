/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 18:33:17 by afelger           #+#    #+#             */
/*   Updated: 2025/03/06 18:41:51 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ms_env_enhance(void)
{
	t_appstate	*state;
	char		**new_env;

	state = get_appstate();
	new_env = malloc(state->env_alloc * 2);
	state->env_alloc *= 2;
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
	state->enviroment[state->env_filled] = ft_mem_reg(ft_strdup(str));
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
	while (++ctr < state->env_filled)
		state->enviroment[ctr - 1] = state->enviroment[ctr];
	state->enviroment[ctr - 1] = NULL;
	state->env_filled--;
}

int	ms_env_init(void)
{
	int			c;
	t_appstate	*state;

	c = 0;
	state = get_appstate();
	state->enviroment = ft_malloc(ENV_ALLOC_SIZE);
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
