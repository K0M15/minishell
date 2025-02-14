/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:05:46 by afelger           #+#    #+#             */
/*   Updated: 2025/02/14 16:07:16 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_elements(char **arr)
{
	int c;
	
	c = 0;
	while (arr[c] != NULL)
		c++;
	return (c);
}

int add_env(char *key, char *value)
{
	t_appstate	*state;
	t_keyvalue	*data;
	t_list		*elem;

	state = get_appstate();
	data = malloc(sizeof(t_keyvalue));
	if (data == NULL)
		return (0);
	data->key = key;
	data->value = value;
	elem = ft_lstnew((void *)data);
	if (elem == NULL)
		return (0);
	if (state->env == NULL)
		state->env = elem;
	else
		ft_lstadd_back(&state->env, elem);
	// printf("%s = %s\n", key, value);
	return (1);
}

int import_env(char **envp)
{
	int		elements;

	elements = count_elements(envp);

	while(--elements)
	{
		int i = 0;
		while(envp[elements][i] != '=')
			i++;
		envp[elements][i++] = 0;
		add_env(envp[elements], &envp[elements][i]);
	}
	return (1);
}

int unset_env(char *key)
{
	//this will maybe create an memory leak since data in old env is not deleted...
	t_list *env = get_appstate()->env;
	t_list *last = NULL;
	while (env != NULL)
	{
		if (ft_strncmp(((t_keyvalue *)env->content)->key,key, ft_strlen(key)) == 0)
		{
			if (last == NULL)
				get_appstate()->env = env->next;
			else
				last->next = env->next;
			free(env->content);
			free(env->next);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

char *get_env(char *key)
{
	t_list *env = get_appstate()->env;
	while (env != NULL)
	{
		if (ft_strncmp(((t_keyvalue *)env->content)->key, key, ft_strlen(key)) == 0)
		{
			return (((t_keyvalue *)env->content)->value);
		}
		env = env->next;
	}
	return (NULL);
}
