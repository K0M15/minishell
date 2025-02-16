/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 10:58:18 by afelger           #+#    #+#             */
/*   Updated: 2025/02/16 13:40:47 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(int argc, char **argv)
{
	int c;

	if (argc == 1)
	{
		printf("unset: not enough arguments\n");
		return (1);
	}
	c = 1;
	while (c < argc)
		ms_delete_value(argc[c]);
	return (0);
}