/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:08:13 by afelger           #+#    #+#             */
/*   Updated: 2025/02/20 09:23:27 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	ft_printf("%s\n", get_appstate()->working_directory);
	return (0);
}
