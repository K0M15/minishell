/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:32:30 by afelger           #+#    #+#             */
/*   Updated: 2025/03/04 15:28:47 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(int argc, char **argv)
{
	int	ctr;
	
	ctr = 0;
	if (argc == 1)
		cleanup(0);
	else if (argc > 2)
	{
		write(2, "exit: too many arguments\n", 25);
		return (1);
	}
	while(argv[1][ctr] && (ft_isdigit(argv[1][ctr]) || argv[1][ctr] == '+' || argv[1][ctr] == '-'))
		ctr++;
	if (argv[1][ctr] == 0)
		cleanup(ft_atoi(argv[1]));
	return (write(2, " numeric argument required\n", 26), 255);
}
