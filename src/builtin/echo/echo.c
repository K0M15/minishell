/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:30:45 by afelger           #+#    #+#             */
/*   Updated: 2025/03/18 17:00:04 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_mode(char *arg)
{
	int	ctr;
	int	result;

	ctr = 1;
	result = 0;
	if (arg && arg[0] == '-')
	{
		while (arg[ctr])
		{
			if (arg[ctr] == 'n')
				result = result | 0b1;
			else
				return (0);
			ctr++;
		}
	}
	return (result);
}

int	builtin_echo(int argc, char **argv)
{
	int		ctr;
	int		mode;

	ctr = 1;
	mode = 0;
	if (argc > 1)
	{
		mode = get_mode(argv[1]);
		while (get_mode(argv[1]) != 0)
			argv++;
		if (argv[1] == NULL)
			return (0);
		if (argc < 2 && mode & 0b1)
			return (0);
		ctr--;
		while (argv[++ctr] && argv[ctr + 1])
			printf("%s ", argv[ctr]);
		printf("%s", argv[ctr]);
	}
	if (!(mode & 0b1))
		printf("\n");
	return (0);
}
