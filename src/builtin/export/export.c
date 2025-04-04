/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:26:16 by afelger           #+#    #+#             */
/*   Updated: 2025/03/28 15:37:23 by afelger          ###   ########.fr       */
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
	char	*key;

	env = get_appstate()->enviroment;
	sort_env(env);
	while (*env)
	{
		if (ms_getvalue(*env))
		{
			key = ms_getkey(*env);
			printf("declare -x %s=\"%s\"\n", key, ms_getvalue(*env));
			free(key);
		}
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

static int	handle_addition(char *key, char *value, char *arg)
{
	char	*copy;

	copy = arg;
	while (*copy && *copy != '+' && *copy != '=')
		copy++;
	if (*copy == '+' && *(copy + 1) == '=')
	{
		copy = ms_get_env(key);
		arg = ft_strjoin(copy, value);
		copy = ms_setvalue(key, arg);
		return (ft_free(arg), 0);
	}
	else if (*copy != 0 && *copy != '=')
	{
		ft_putstr_fd("bash: export: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": not a valid identifier\n", 2);
		return (1);
	}
	else
		return (ms_setvalue(key, value), 0);
}

int	builtin_export(int argc, char **argv)
{
	char	*key;
	char	*value;
	int		c;
	int		retval;

	retval = 0;
	if (argc == 1)
		return (export_displayx(), 0);
	c = 0;
	while (++c < argc)
	{
		key = export_getkey(argv[c]);
		if (key[0] == 0 || contains_illegal(key))
		{
			ft_putstr_fd("bash: export: `", 2);
			ft_putstr_fd(argv[c], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return (ft_free(key), 1);
		}
		value = ms_getvalue(argv[c]);
		retval |= handle_addition(key, value, argv[c]);
		ft_free(key);
	}
	return (retval);
}
