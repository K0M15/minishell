/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:32:30 by afelger           #+#    #+#             */
/*   Updated: 2025/03/23 11:32:00 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "unistd.h"

static int	only_accepted(char *str)
{
	size_t	strl;

	strl = ft_strlen(str) > 0;
	if (*str == '-' || *str == '+')
		str++;
	while (*str && ft_isdigit(*str))
		str++;
	if (*str == 0 && strl)
		return (0);
	return (255);
}

static int handle_exit(int code, char *err_reson)
{
	get_appstate()->stop = 1;
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 2);
	if (err_reson != NULL)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(err_reson, 2);
		ft_putstr_fd("\n", 2);
	}
	return (code & 0xFF);
}

static void print_err_numeric(char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

int	builtin_exit(int argc, char **argv)
{
	int	ctr;

	ctr = 0;
	if (argc == 1)
		return (handle_exit(0, NULL));
	if (only_accepted(argv[1]))
	{
		print_err_numeric(argv[1]);
		return (handle_exit(255, NULL));
	}
	if (argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	while (argv[1][ctr] && (ft_isdigit(argv[1][ctr]) \
		|| argv[1][ctr] == '+' || argv[1][ctr] == '-'))
		ctr++;
	if (argv[1][ctr] == 0)
		return (handle_exit(ft_atoi(argv[1]), NULL));
	print_err_numeric(argv[1]);
	return (handle_exit(255, "numeric argument required"));
}
