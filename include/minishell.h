/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:00:44 by afelger           #+#    #+#             */
/*   Updated: 2025/02/16 14:02:35 by afelger          ###   ########.fr       */
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

# define ENV_ALLOC_SIZE 1024;

typedef struct s_appstate
{
	char	**enviroment;
	size_t	env_alloc;
	size_t	env_filled;
	char	last_return;
	char	*working_dirctory;
}	t_appstate;

t_appstate *get_appstate();

int			pwd(int argc, char **argv);
int			env(int argc, char **argv);

int			ms_delete_value(char *key);
char		*ms_setvalue(char *key, char *value);
char		*ms_getvalue(char *key);
char		*ms_getindex(char *key);
char		*ms_getvalue(char *str);
int			ms_is_key(char *str, char *key);
char		*ms_getkey(char *str);

char		**ms_env_enhance();
int			ms_env_append(char *str);
void		ms_env_delete(int id);

#endif // MINISHELL_H