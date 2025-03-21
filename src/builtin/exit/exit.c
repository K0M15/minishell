/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:32:30 by afelger           #+#    #+#             */
/*   Updated: 2025/03/21 12:54:48 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	only_accepted(char *str)
{
	size_t	strl;

	strl = ft_strlen(str) > 0;
	if (*str == '-')
		str++;
	while (*str && ft_isdigit(*str))
		str++;
	if (*str == 0 && strl)
		return (0);
	write(2, "bash: exit: some: numeric argument required\n", 44);
	return (1);
}

int	builtin_exit(int argc, char **argv)
{
	int	ctr;

	ctr = 0;
	get_appstate()->stop = 1;
	if (argc == 1)
		return (0);
	if (only_accepted(argv[1]))
		return (1);
	if (argc > 2)
	{
		write(2, "exit: too many arguments\n", 25);
		return (1);
	}
	while (argv[1][ctr] && (ft_isdigit(argv[1][ctr]) \
		|| argv[1][ctr] == '+' || argv[1][ctr] == '-'))
		ctr++;
	if (argv[1][ctr] == 0)
		return (ft_atoi(argv[1]));
	return (write(2, " numeric argument required\n", 26), 255);
}
