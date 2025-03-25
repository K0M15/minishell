/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:26:16 by afelger           #+#    #+#             */
/*   Updated: 2025/03/25 15:43:49 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*export_getkey(char *str)
{
	int		c;
	char	*result;

	c = 0;
	while (str[c] && str[c] != '=' && str[c] != '+')
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

static void	export_displayx(void)
{
	char	**env;

	env = get_appstate()->enviroment;
	while (*env)
	{
		printf("declare -x %s\n", *env);
		env++;
	}
}

static int	contains_illegal(char *str)
{
	if (!(ft_isalpha(*str) || *str == '_' || *str == '-'))
		return (1);
	while (*str)
	{
		if (!(ft_isalnum(*str) || *str == '_'))
			return (1);
		str++;
	}
	return (0);
}

static char *handle_addition(char *key, char *value, char *arg)
{
	char *copy;

	copy = arg;
	while (*copy && (*copy != '+' && *copy != '='))
		copy++;
	if (*copy == '+')
	{
		copy = ms_get_env(key);
		arg = ft_strjoin(copy, value);
		copy = ms_setvalue(key, arg);
		return (ft_free(arg), copy);
	}
	else
		return (ms_setvalue(key, value));
}

int	builtin_export(int argc, char **argv)
{
	char	*key;
	char	*value;
	int		c;

	if (argc == 1)
		return (export_displayx(), 0);
	c = 1;
	while (c < argc)
	{
		key = export_getkey(argv[c]);
		if (key[0] == 0 || contains_illegal(key))
		{
			write(2, "bash: export: `", 15);
			write(2, argv[c], ft_strlen(argv[c]));
			write(2, "': not a valid identifier\n", 26);
			free(key);
			return (1);
		}
		value = ms_getvalue(argv[c]);
		if (handle_addition(key, value, argv[c]) == NULL)
			perror("bash: export: ");
		free(key);
		c++;
	}
	return (0);
}
