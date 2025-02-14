/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:59:31 by afelger           #+#    #+#             */
/*   Updated: 2025/02/14 12:34:09 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

const char *tokenNames[] = {
	"UNDEFINED",
	"OPERATOR",
	"SYMBOL",
	// "SOFT_STRING",
	// "HARD_STRING",
	// "PARANTHESES_OPEN",	// (
	// "PARANTHESES_CLOSE",	// (
	// "BRACKETS_OPEN",		// [
	// "BRACKETS_CLOSE",		// ]
	// "CURLY_OPEN",			// {
	// "CURLY_CLOSE",			// }
};

void warn(char *msg)
{
	write(1, KYEL, ft_strlen(KYEL));
	write(1, msg, ft_strlen(msg));
	write(1, KNRM "\n", ft_strlen(KNRM)+1);
}

char *get_prompt()
{
	warn("GET_PROMP Prototype");
	return ("<$USER@$MACHINE $FOLDERNAME>");
}

typedef enum e_tokentype
{
	UNDEFINED = 0,
	OPERATOR,
	SYMBOL,
	// SOFT_STRING,
	// HARD_STRING,
	// PARANTHESES_OPEN,	// (
	// PARANTHESES_CLOSE,	// (
	// BRACKETS_OPEN,		// [
	// BRACKETS_CLOSE,		// ]
	// CURLY_OPEN			// {
	// CURLY_CLOSE			// }
}	t_tokentype;

typedef struct s_token
{
	t_tokentype	type;
	size_t		length;
	char		*content;
}	t_token;

int ms_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\v' || c == '\f')
		return 1;
	return 0;
}

char *ft_lstrip(char *str)
{
	while (str && ms_isspace(*str))
		str++;
	return (str);
}

int ms_iscrtl(char c)
{
	char *crtl;
	
	crtl = "<>({[]})\"'";
	while (*crtl)
		if (*crtl == c)
			return (1);
		else
			crtl++;
	return (0);
}

t_token *getToken(char **str)
{
	char *crt;
	t_token *token;

	token = malloc(sizeof(t_token));
	crt = ft_lstrip(*str);
	if (ms_iscrtl(*crt))
	{
		warn("NOT IMPLEMENTED TOKEN");
		token->length = 1;
		token->type = UNDEFINED;
	}
	else if (ft_isalnum(*crt))
	{
		token->type = SYMBOL;
		token->length = 0;
		while(!ms_isspace(crt[token->length]) && !ms_iscrtl(crt[token->length]) && crt[token->length])
			token->length++;
	}
	token->content = malloc(token->length + 1);
	ft_memcpy(token->content, crt, token->length);
	token->content[token->length] = 0;
	*str = crt + token->length;
	return (token);
}

t_list *tokenize(char *str)
{
	size_t	position;
	t_list	*result;

	position = 0;
	result = ft_lstnew(getToken(&str));
	while (*str)
	{
		ft_lstadd_back(&result, (void *)getToken(&str));
	}
	return (result);
}

void print_token(t_token *token)
{
	printf("(%s) (%zu) %s\n", tokenNames[token->type], token->length, token->content);
	free(token->content);
	free(token);
}

int main()
{
	char *str;
	t_list *tokens;
	while (1)
	{
		str = readline(get_prompt());
		tokens = tokenize(str);
		free(str);
		ft_lstclear(&tokens, (void (*)(void *))&print_token);
	}
}