/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:00:44 by afelger           #+#    #+#             */
/*   Updated: 2025/02/14 17:30:07 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stddef.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "ft_printf.h"
# include "stdlib.h"

typedef struct s_appstate
{
	t_list	*env;
	char	last_return;
}	t_appstate;

typedef struct s_keyvalue
{
	char *key;
	char *value;
}	t_keyvalue;

size_t	count_elements(char **arr);
int add_env(char *key, char *value);
int import_env(char **envp);
int unset_env(char *key);
char *get_env(char *key);
t_appstate *get_appstate();

int	pwd(void);
int env(void);

#endif // MINISHELL_H