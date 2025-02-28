/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 10:58:18 by afelger           #+#    #+#             */
/*   Updated: 2025/02/28 15:14:23 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(int argc, char **argv)
{
	int	c;

	if (argc == 1)
	{
		printf("unset: not enough arguments\n");
		return (1);
	}
	c = 0;
	while (++c < argc)
		ms_delete_value(argv[c]);
	return (0);
}
