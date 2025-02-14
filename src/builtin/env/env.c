/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:24:59 by afelger           #+#    #+#             */
/*   Updated: 2025/02/14 17:30:09 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int env(void)
{
	t_list *enviroment;
	t_keyvalue *kvpair;

	enviroment = get_appstate()->env;
	while(enviroment != NULL)
	{
		kvpair = (t_keyvalue *)enviroment->content;
		ft_printf("%s=%s\n", kvpair->key, kvpair->value);
		enviroment = enviroment->next;
	}
	return (1);
}

// int main(int argc, char **argv, char **penv)
// {
// 	(void)argc;
// 	(void)argv;
// 	import_env(penv);
// 	env();
// }