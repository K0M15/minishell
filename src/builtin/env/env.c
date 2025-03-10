/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:24:59 by afelger           #+#    #+#             */
/*   Updated: 2025/03/04 15:19:51 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(int argc, char **argv)
{
	char	**enviroment;

	(void) argc;
	(void) argv;
	enviroment = get_appstate()->enviroment;
	while (*enviroment)
	{
		if (ms_getvalue(*enviroment)[0] != 0)
			ft_printf("%s\n", *enviroment);
		enviroment++;
	}
	return (0);
}

// int main(int argc, char **argv, char **penv)
// {
// 	(void)argc;
// 	(void)argv;
// 	import_env(penv);
// 	env();
// }