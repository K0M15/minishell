/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:26:16 by afelger           #+#    #+#             */
/*   Updated: 2025/02/16 14:01:21 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	export(int argc, char **argv)
{
	char	*key;
	char	*value;
	int		c;

	if (argc == 1)
	{
		//display declare-x
		return (0);
	}
	
	c = 1;
	while (c < argc)
	{
		key = ms_getkey(argv[c]);
		value = ms_getvalue(argv[c]);
		ms_setvalue(key, value);
		free(key);
		c++;
	}
	return (0);
}