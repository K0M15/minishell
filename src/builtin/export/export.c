/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:26:16 by afelger           #+#    #+#             */
/*   Updated: 2025/03/03 13:43:28 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	export_displayX(void)
{
	char	**env;

	env = get_appstate()->enviroment;
	while (*env)
	{
		printf("declare -x %s\n", *env);
		env++;
	}
}

static int contains_illegal(char *str)
{
	if (!(ft_isalpha(*str) || *str == '_'))
		return (1);
	while (*str)
	{
		if (!(ft_isalnum(*str) || *str == '_'))
			return (1);
		str++;
	}
	return (0);
}

int	builtin_export(int argc, char **argv)
{
	char	*key;
	char	*value;
	int		c;

	if (argc == 1)
		return (export_displayX(), 0);
	c = 1;
	while (c < argc)
	{
		key = ms_getkey(argv[c]);
		if (key[0] == 0 || contains_illegal(key))
		{
			write(2, "minishell: export: \"", 20);
			write(2, key, ft_strlen(key));
			write(2, "\": not a valid identifier\n", 26);
			free(key);
			return (1);
		}
		value = ms_getvalue(argv[c]);
		ms_setvalue(key, value);
		free(key);
		c++;
	}
	return (0);
}
