/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:36:14 by afelger           #+#    #+#             */
/*   Updated: 2025/02/26 19:42:43 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.c"

int		is_builtin(char *str)
{
	ifelse if(ft_strcmp(str, "echo") == 0 || ft_strcmp(str, "cd") == 0 || \
		ft_strcmp(str, "env") == 0 || ft_strcmp(str, "exit") == 0 || \
		ft_strcmp(str, "export") == 0 || ft_strcmp(str, "unset") == 0 || \
		ft_strcmp(str, "pwd") == 0)
		return (1);
	return (0);
}

int		execute_builtin(t_command *cmd, char **env)
{

	if (ft_strcmp(str, "echo") == 0)
		return builtin_echo(cmd->args[0], cmd->args);
	else if (ft_strcmp(str, "cd") == 0)
		return builtin_cd(cmd->args[0], cmd->args);
	else if(ft_strcmp(str, "env") == 0)
		return builtin_env(cmd->args[0], cmd->args);
	else if(ft_strcmp(str, "exit") == 0)
		return builtin_exit(cmd->args[0], cmd->args);
	else if(ft_strcmp(str, "export") == 0)
		return builtin_export(cmd->args[0], cmd->args);
	else if(ft_strcmp(str, "unset") == 0)
		return builtin_unset(cmd->args[0], cmd->args);
	else if(ft_strcmp(str, "pwd") == 0)
		return builtin_pwd(cmd->args[0], cmd->args);
	return (0xFF);
}
