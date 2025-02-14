/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:26:16 by afelger           #+#    #+#             */
/*   Updated: 2025/02/14 17:22:26 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void ft_strip(char **elem)
{
	int c;

	c = 0;
	while(**elem == ' ' || **elem == '\t' || **elem == '\v' || **elem == '\r' || **elem == '\n')
		(*elem)++;
	while(ft_isalnum((*elem)[c]) && (*elem)[c])
		c++;
	elem[c] = 0;
}

int export(char *str)
{
	//handle empty value strings
	//handle empty keys
	//what else could be entered?
	//no = sign
	//are numbers allowed?
	//must the keys be uppercase?
	char *key;
	char *value;

	key = str;
	while(*str != '=')
		str++;
	*str = 0;
	value = str + 1;
	ft_strip(&value);
	ft_strip(&key);
	return (add_env(key, value));
}

int main(int argc, char **argv)
{
	// a smol test function
	if (argc < 2)
		return (1);
	int c = 1;
	while (c < argc)
	{
		export(argv[c]);
		c++;
	}
	t_list *env;
	env = get_appstate()->env;
	while(env != NULL) {
		printf("%s = %s\n", ((t_keyvalue *)env->content)->key, ((t_keyvalue *)env->content)->value);
		env = env->next;
	}
}