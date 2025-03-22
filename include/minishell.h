/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:00:44 by afelger           #+#    #+#             */
/*   Updated: 2025/03/22 18:23:03 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stddef.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <ctype.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>

# include "libft.h"
# include "ft_printf.h"
# include "ft_malloc.h"
# include "get_next_line.h"
# include "ft_dyn_str.h"

typedef struct s_doc
{
	struct s_doc	*next;
	char			*content;
	int				length;
}	t_doc;

typedef enum e_tokentype
{
	TOKEN_WORD,				// Words and quoted strings
	TOKEN_PIPE,				// |
	TOKEN_REDIRECT_IN,		// <
	TOKEN_REDIRECT_OUT,		// >
	TOKEN_APPEND_OUT,		// >>
	TOKEN_HERE_DOCUMENT,	// <<
	TOKEN_EOF,				// End of input
	// TOKEN_DOUBLEAND,		// &&
	// TOKEN_DOUBLEPIPE,		// ||
	// TOKEN_PARATESES_OPEN,	// (
	// TOKEN_PARATESES_CLOSE,	// )
}	t_tokentype;


typedef struct s_token
{
	t_tokentype		type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_lexer
{
	char			*input;
	size_t			input_len;
	size_t			pos;
	bool			in_dd_quote;
	bool			in_s_quote;
	bool			contains_var;
}	t_lexer;

// Command types
typedef enum e_commandtype
{
	CMD_SIMPLE,	// Simple command (e.g., "ls -l")
	CMD_PIPE,	// Pipeline (e.g., "ls | grep foo")
	CMD_REDIR	// Command with redirections
}	t_commandtype;

typedef enum e_redirtype
{
	REDIR_IN,		// Input redirection (<)
	REDIR_OUT,		// Output redirection (>)
	REDIR_APPEND,	// Append output (>>)
	REDIR_HEREDOC	// Here document (<<)
}	t_redirtype;

typedef struct s_redirection
{
	t_redirtype				type;
	char					*file; // Target file/delimiter
	struct s_redirection	*next; // Next redirection
	int						fd;
	int						has_quotes;
}	t_redirection;

// Command structure (node in the AST)
typedef struct s_command
{
	t_commandtype		type;

	char				**args;
	size_t				args_cap;
	struct s_command	*left;
	struct s_command	*right;
	t_redirection		*redirections;
	pid_t				pid;
	pid_t				pid2;
	t_doc				*heredoc;
	bool				canceled;
}	t_command;

#define INITIAL_ARG_CAPACITY 16

# define ENV_ALLOC_SIZE 1024
# define HISTORY_FILENAME "hist_dump"

typedef struct sigaction t_sigaction;

extern char		**environ;

extern volatile sig_atomic_t	g_ms_signal;

typedef enum e_appmode
{
	INTERACTIVE = 0,
	HEREDOC,
	RUNNING
}	t_appmode;

typedef struct s_appstate
{
	char			**enviroment;
	char			*appname;
	size_t			env_alloc;
	size_t			env_filled;
	t_list			*children;
	t_appmode		active_mode;
	int				rainbow;
	volatile bool	cancled_heredoc;
	int				last_return;
	int				stop;
}	t_appstate;

t_appstate		*get_appstate(void);
/*
	Sets the mode of get_appstate
	Changes interrupts appropriate
*/
void			ms_set_state_mode(t_appmode mode);


//============================================		BUILTINS
int				builtin_pwd(int argc, char **argv);
int				builtin_env(int argc, char **argv);
int				builtin_echo(int argc, char **argv);
int				builtin_env(int argc, char **argv);
int				builtin_exit(int argc, char **argv);
int				builtin_export(int argc, char **argv);
int				builtin_unset(int argc, char **argv);
int				builtin_cd(int argc, char **argv);
int				is_builtin(char *str);
int				execute_builtin(t_command *cmd, char **env);
//============================================		END BUILTINS

//	0 if not found, 1 if deleted
int				ms_delete_value(char *key);
//	returns new enty string if inserted or NULL if error occured
char			*ms_setvalue(char *key, char *value);
//	Return value either the value of key or a non-freeable empty string if not found
char			*ms_get_env(char *key);
//	Returns the index of the char * object inside the enviroment
int				ms_getindex(char *key);
//	Extracts the value of key=value in str
char			*ms_getvalue(char *str);
//	Checks if key of str equals key
int				ms_is_key(char *str, char *key);
//	Extracts the key of key=value in str. Needs to be freed
char			*ms_getkey(char *str);

/**
 * 	Enhances the enviroment array by allocating a new, double the size,
 * 	and copying the values,
 * 	then freeing the old and replacing the value in get_appstate()
 */
char			**ms_env_enhance(void);
//	Appends a key to get_appstate()->enviroment
int				ms_env_append(char *str);
/*
	Deletes a position inside of get_appstate()->enviroment
	by shifting all elements after it one place forward
*/
void			ms_env_delete(int id);
//	Initializes the appstate enviroment with variables from existing enviroment
int				ms_env_init(void);

//	Display MINISHELL welcome msg
void			ms_display_welcome(void);
//	get prompt string
char			*ms_get_prompt(void);

// get a token type representation for errors. If word, it returns value of token
const char		*look_token_rep(t_token *token);

//	Writes out history entry to fd
int				write_hist_entry(int fd, HIST_ENTRY *entry);
/*
	Replaces "search" inside of "content" with "replace"
	If Truncate is not 0, all occurences from the right
	of "search" will be replaced with 0 until the first
	char that is not "search". Then they will be replaced
	with "replace"
*/
void 			ft_replace(char *content, char search, char replace, int truncate);
/* Will load filecontent into rl_history */
void			load_history(char *filename);
/*
	Will dump rl_history into file. If file not
	exists, create new.
	If an error occurs, it will return -1
*/
int				dump_history(char *filname);

void			init_terminal(void);
void			init_child_terminal(void);

// =================   STILL TODO   ================= //
/**
 * Three different modes to check:					Behaviour
 * 	-	interactive mode (no execve running)		cancle line, new prompt, not in history
 * 	-	heredoc mode								cancle heredoc and command. Command stays in history
 *  -	process running inside of shell				send SIGINT to process, display output, should stay in history. Maybe display "^C\n"
 */
//	Initializes the signalhandling
void			ms_sig_init(void);
//	Returns a pointer to the current sigaction
t_sigaction		*ms_get_sig_action(void);
//	Interupt handler in interactive mode
void			ms_sig_handler_interactive(int signal, siginfo_t *info, void *ctx);
//	Interupt handler in heredoc mode
void			ms_sig_handler_heredoc(int signal, siginfo_t *info, void *ctx);
//	Interupt handler in running mode
void			ms_sig_handler_running(int signal, siginfo_t *info, void *ctx);
//	Kills all the attached processes
int				ms_sig_kill_all(t_list *processes, int signal);
//	Kills a single process
int				ms_sig_kill(t_command *process, int signal);

//============================================	HEREDOC
//	executes heredoc. dellimter = EOF
int				ms_heredoc(const char *delimiter, int no_var_exp);
t_doc			*ms_doc_app_or_new(struct s_doc **document);
int				ms_doc_display_free(struct s_doc *document, int fd, int no_var_exp);
char			*ms_doc_construct(struct s_doc *document);
int				ms_doc_get_length(struct s_doc *document);
int				ms_doc_append(struct s_doc *document, struct s_doc **last);
void			ms_doc_free(t_doc *document);

//============================================	END HEREDOC


//============================================
// LEXER INITIALIZATION AND UTILITY FUNCTIONS
//============================================

/**
 * Creates a new lexer with the string as input
 * @param char *input String to lex
 * @return Lexer
 */
t_lexer *init_lexer(char *input);

char    current_char(t_lexer *lexer);
char    peek_next(t_lexer *lexer);
void    advance(t_lexer *lexer);
int     ft_isspace(char c);
int     is_operator_char(char c);
void    skip_whitespace(t_lexer *lexer);

//============================================
// TOKEN HANDLING AND CREATION
//============================================

/**
 * Creates a token of type and with a copy of value
 */
t_token *create_token(t_tokentype type, char *value);
t_token *get_next_token(t_lexer *lexer);
void    free_tokens(t_token *head);
void    add_token_to_list(t_token **head, t_token **current, t_token *token);

/**
 * Tokenizes the input and returns a linked list of tokens
 */
t_token *tokenize(char *input);
t_token *build_token_list(t_lexer *lexer);

//============================================
// OPERATOR AND SPECIAL TOKEN HANDLERS
//============================================

/**
 * Identifies operators and delegates to the appropriate function
 */
t_token *handle_operator(t_lexer *lexer);
t_token *handle_pipe(t_lexer *lexer);
t_token *handle_redirect_out(t_lexer *lexer);
t_token *handle_redirect_in(t_lexer *lexer);
int     is_redirection_token(t_tokentype type);

//============================================
// WORD AND VARIABLE HANDLING
//============================================

t_token *handle_word(t_lexer *lexer);
t_token *handle_quote(t_lexer *lexer);
char    *handle_variable(t_lexer *lexer);
char    *expand_variables_in_string(const char *str);
void    process_quote_content(t_lexer *lexer, char *buffer, int *i, char quote_char);
void    process_variable_name(t_lexer *lexer, char *buffer, int *i, bool has_braces);
void    process_variable_in_word(t_lexer *lexer, char *buffer, int *i);
void    process_quotes_in_word(t_lexer *lexer, char *buffer, int *i);
void    process_variable_in_quotes(t_lexer *lexer, char *buffer, int *i, char quote_char);
char    *handle_quotes(char *str);
char	*handle_quotes_redir(char *str);

//============================================
// DYNAMIC STRING AND VARIABLE EXPANSION
//============================================

int     add_variable(t_dyn_str *result, char *str, long *pos);
int     handle_exit_status(t_dyn_str *result, long *pos);
void    handle_exit_status_var(char *result, size_t *i);
void    handle_env_var(const char *str, char *result, size_t *i);
void    process_char(t_dyn_str *result, char *str, long *ctr, int *in_quotes);
void    handle_quote_state(char c, int *in_quotes);

//============================================
// PARSING AND AST CONSTRUCTION
//============================================

/**
 * Main parsing function that constructs the AST from tokens
 */
t_command *parse(t_token *tokens);
t_command *parse_pipeline(t_token **tokens);
t_command *parse_simple_command(t_token **tokens);
t_redirection *parse_redirection(t_token **tokens);
int       process_command_tokens(t_token **current, t_command *cmd);
int       handle_word_token(t_token **current, t_command *cmd);

//============================================
// COMMAND AND REDIRECTION CREATION
//============================================

t_command		*create_simple_command(void);
t_command		*initialize_simple_command(void);
t_command		*create_pipe_command(t_command *left, t_command *right);
void			add_argument(t_command *cmd, const char *arg);
t_redirection	*create_redirection(t_redirtype type, const char *file, int has_quotes);
void			add_redirection(t_command *cmd, t_redirection *redir);
t_redirection	*handle_redirection_token(t_token **current, t_command *cmd);
void			free_command(t_command *cmd);

//============================================
// COMMAND EXECUTION
//============================================

/**
 * Executes a command node from the AST
 */
int			execute_command(t_command *cmd, char **env, int fork);
int			execute_simple_command(t_command *cmd, char **env, int fork);
int			execute_pipe_command(t_command *cmd, char **env);
int			execute_builtin(t_command *cmd, char **env);
int			is_builtin(char *str);
void		expand_variables(t_command *cmd, char **env);

//============================================
// REDIRECTION HANDLING
//============================================

int			apply_redirections(t_command *cmd);
int			redirection_in(const char *file);
int			redirection_out(const char *file);
int			redirection_append(const char *file);
int			handle_redirection_type(t_redirtype type, const char *file, int has_quotes);
int			apply_heredoc(const char *dellimiter, int no_variable_exp);
int			save_fds(int *saved_fds);
void		restore_fds(int saved_fds[3]);
int			redir_replace_fd(t_redirection *r);

//============================================
// PROCESS MANAGEMENT
//============================================

pid_t		ft_fork(void);
int			create_pipe(int *pipefd);
pid_t		fork_and_execute_left(t_command *cmd, char **env, int *pipefd);
pid_t		fork_and_execute_right(t_command *cmd, char **env, int *pipefd);
int			execute_child_process(t_command *cmd);
int			execute_forked_command(t_command *cmd, int *saved_fds);
int			handle_builtin_or_redirections(t_command *cmd, char **env, int *saved_fds);
void		handle_execve_error(char *cmd);

//============================================
// UTILITY FUNCTIONS
//============================================

char		*find_path(char *cmd);
void		free_string_arr(char **arr);
char		*ft_strcpy(char *dst, const char *src);
void		ft_strncat(char *target, const char *source, size_t amount, size_t max);
char		*ft_strndup(char *dst, const char *src, size_t n);
void		*ft_realloc(void *ptr, size_t old_size, size_t new_size);
int			ft_strlencmp(const char *s1, const char *s2);
void		toggle_quote(int *quote_flag);
int			count_args(char **args);
int			is_directory(const char *path);
int			is_executable(const char *path);
char		*exists(char *path);

#endif // MINISHELL_H
