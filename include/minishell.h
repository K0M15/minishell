/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:00:44 by afelger           #+#    #+#             */
/*   Updated: 2025/02/20 08:53:20 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stddef.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdlib.h>

# include "libft.h"
# include "ft_printf.h"
# include "ft_malloc.h"

# define ENV_ALLOC_SIZE 1024

extern char						**environ;

extern volatile sig_atomic_t	g_ms_signal;

typedef struct s_command
{
	void			*alloc;
	char			*input;
	void			*tokens;
	void			*ast;
	unsigned int	pid;
	unsigned int	ret_value;
}	t_command;

typedef struct s_appstate
{
	void	*alloc;
	char	**enviroment;
	size_t	env_alloc;
	size_t	env_filled;
	char	*working_directory;
	t_list	*children;
}	t_appstate;

t_appstate	*get_appstate();

int			builtin_pwd(int argc, char **argv);
int			builtin_env(int argc, char **argv);
int			builtin_echo(int argc, char **argv);
int			builtin_env(int argc, char **argv);
int			builtin_exit(int argc, char **argv);
int			builtin_export(int argc, char **argv);
int			builtin_unset(int argc, char **argv);
int			builtin_cd(int argc, char **argv);

//	0 if not found, 1 if deleted
int			ms_delete_value(char *key);
//	returns new enty string if inserted or NULL if error occured
char		*ms_setvalue(char *key, char *value);
//	Return value either the value of key or NULL if not found
char		*ms_get_env(char *key);
//	Returns the index of the char * object inside the enviroment
int			ms_getindex(char *key);
//	Extracts the value of key=value in str
char		*ms_getvalue(char *str);
//	Checks if key of str equals key
int			ms_is_key(char *str, char *key);
//	Extracts the key of key=value in str. Needs to be freed
char		*ms_getkey(char *str);

/**
 * 	Enhances the enviroment array by allocating a new, double the size,
 * 	and copying the values,
 * 	then freeing the old and replacing the value in get_appstate()
 */
char		**ms_env_enhance(void);
//	Appends a key to get_appstate()->enviroment
int			ms_env_append(char *str);
/*	
	Deletes a position inside of get_appstate()->enviroment
	by shifting all elements after it one place forward
*/
void		ms_env_delete(int id);
//	Initializes the appstate enviroment with variables from existing enviroment
int			ms_env_init(void);

//	Display MINISHELL welcome msg
void		ms_display_welcome(void);
//	get prompt string
char		*ms_get_prompt(void);

// =================   STILL TODO   ================= //
//	Initializes the signalhandling
void		ms_sig_init(void);
/**
 * Three different modes to check:					Behaviour
 * 	-	interactive mode (no execve running)		cancle line, new prompt, not in history
 * 	-	heredoc mode								cancle heredoc and command. Command stays in history
 *  -	process running inside of shell				send SIGINT to process, display output, should stay in history. Maybe display "^C\n"
 */

#endif // MINISHELL_H
