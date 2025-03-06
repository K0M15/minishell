/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 10:58:18 by afelger           #+#    #+#             */
/*   Updated: 2025/03/06 13:38:49 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int contains_illegal(char *str)
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

int	builtin_unset(int argc, char **argv)
{
	int	c;

	if (argc == 1 && isatty(STDIN_FILENO))
	{
		printf("bash: unset: not enough arguments\n");
		return (1);
	}
	c = 0;
	while (++c < argc)
	{
		if (contains_illegal(argv[c]))
		{
			write(2, "bash: unset: `", 14);
			write(2, argv[c], ft_strlen(argv[c]));
			write(2, "': not a valid identifier\n", 26);
			return (1);
		}
		ms_delete_value(argv[c]);
	}
	return (0);
}
