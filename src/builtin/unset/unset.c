/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 10:58:18 by afelger           #+#    #+#             */
/*   Updated: 2025/03/06 18:32:02 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			ft_putstr_fd("bash: unset: `", 2);
			ft_putstr_fd(argv[c], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return (1);
		}
		ms_delete_value(argv[c]);
	}
	return (0);
}
