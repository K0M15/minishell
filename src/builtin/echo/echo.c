/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:30:45 by afelger           #+#    #+#             */
/*   Updated: 2025/02/28 14:44:13 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(int argc, char **argv)
{
	int		ctr;
	size_t	strl;
	int		has_minus_n;

	(void) argc;	
	ctr = 0;
	strl = 0;
	has_minus_n = argc < 3 && \
		ft_strncmp("-n", argv[1], 2) && ft_strlen(argv[1]) == 2;	//Handle -nnnnnn
	if (argc < 2 || has_minus_n)
		return (0);
	while (argv[++ctr])
		ft_printf("%s ", argv[ctr]);
	if (!has_minus_n)
		ft_printf("\n");
	return (0);
}
