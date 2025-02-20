/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:30:45 by afelger           #+#    #+#             */
/*   Updated: 2025/02/20 09:19:53 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(int argc, char **argv)
{
	int		ctr;
	size_t	strl;
	int		has_minus_n;

	(void) argc;	
	ctr = 1;
	strl = 0;
	has_minus_n = argc < 3 && \
		ft_strncmp("-n", argv[1], 2) && ft_strlen(argv[1]) == 2;
	if (argc < 2 || has_minus_n)
		return (0);
	while (argv[ctr] && argv[ctr + 1])
		printf("%s ", argv[ctr]);
	printf("%s", argv[ctr]);
	if (!has_minus_n)
		printf("\n");
	return (0);
}
