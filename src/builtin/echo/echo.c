/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:30:45 by afelger           #+#    #+#             */
/*   Updated: 2025/03/01 09:55:39 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_mode(char *arg)
{
	int ctr;
	int result;

	ctr = 1;
	result = 0;
	if (arg[0] == '-')
	{
		while(arg[ctr])
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
	size_t	strl;
	int		mode;

	ctr = 1;
	strl = 0;
	mode = 0;
	if (argc > 1)
	{
		mode = get_mode(argv[1]);
		if (mode)
			argc++;
		if (argc < 2 || mode & 0b1)
			return (0);
		while (argv[ctr] && argv[ctr + 1])
			printf("%s ", argv[ctr]);
		printf("%s", argv[ctr]);

	}
	if (!(mode & 0b1))
		printf("\n");
	return (0);
}
