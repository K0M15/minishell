/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:32:30 by afelger           #+#    #+#             */
/*   Updated: 2025/03/03 14:01:31 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(int argc, char **argv)
{
	if (argc == 1)
		cleanup(0);
	else if (argc > 2)
	{
		write(2, "exit: too many arguments\n", 25);
		return (1);
	}
	else
		cleanup(ft_atoi(argv[1]));
	return (0);
}
