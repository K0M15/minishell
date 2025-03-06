/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:26:16 by afelger           #+#    #+#             */
/*   Updated: 2025/03/06 18:31:33 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		key = ms_getkey(argv[c]);
		if (key[0] == 0 || contains_illegal(key))
		{
			write(2, "bash: export: `", 15);
			write(2, argv[c], ft_strlen(argv[c]));
			write(2, "': not a valid identifier\n", 26);
			free(key);
			return (1);
		}
		value = ms_getvalue(argv[c]);
		if (ms_setvalue(key, value) == NULL)
			perror("bash: export: ");
		free(key);
		c++;
	}
	return (0);
}
