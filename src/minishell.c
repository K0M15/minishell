/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:59:31 by afelger           #+#    #+#             */
/*   Updated: 2025/02/14 16:08:10 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"




int main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	import_env(envp);
	printf("%s\n", get_env("USER"));
	printf("%s\n", get_env("LANG"));
	printf("%s\n", get_env("PATH"));
	printf("%s\n", get_env("LOGNAME"));
	// char *str;
	// while (1)
	// {
	// 	str = readline(">");
	// 	free(str);
	// }
}