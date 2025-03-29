/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:08:13 by afelger           #+#    #+#             */
/*   Updated: 2025/03/06 18:31:41 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(int argc, char **argv)
{
	char	*cwd;

	(void) argc;
	(void) argv;
	cwd = getcwd(NULL, 0);
	ft_printf("%s\n", cwd);
	ft_free(cwd);
	return (0);
}
