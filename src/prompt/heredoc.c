/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:38:08 by afelger           #+#    #+#             */
/*   Updated: 2025/02/22 18:41:52 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// void eval_variables(char *str)
// {
	
// }

int ms_doc_append(struct s_doc *document, struct s_doc **last)
{
	while (document != NULL)
		document = document->next;
	document = malloc(sizeof(struct s_doc));
	if (document == NULL)
		return (-1);
	document->next = NULL;
	*last = document;
	return (0);
}

int ms_doc_get_length(struct s_doc *document)
{
	int result;
	
	result = 0;
	while (document != NULL)
	{
		result += document->length;
		document = document->next;
	}
	return (result);
}

char *ms_doc_construct(struct s_doc *document)
{
	int length;
	char *str;

	length = ms_doc_get_length(document);
	str = malloc(length);
	if (str == NULL)
		return NULL;
	while (document != NULL)
	{
		printf("%s\n", document->content);
		ft_strlcat(str, document->content, document->length);
		document = document->next;
	}
	return (str);
}

void ms_doc_free(struct s_doc *document)
{
	struct s_doc *next;

	while (document != NULL)
	{
		next = document->next;
		free(document->content);
		free(document);
		document = next;
	}
}

int ms_heredoc(char *dellimter, int fd, struct s_doc *document, int exit)
{
	// not directly piped but added to array, then sent
	// linked list should also be possible. 
	char			*str;
	struct s_doc	*last;
	int				iseof;
	//new line all that good stuff
	if(ms_doc_append(document, &last) == -1)
		return (-1);
	str = readline("heredoc> ");
	iseof = (ft_strncmp(str, dellimter, ft_strlen(dellimter)) == 0);
	if (iseof && exit == 0)
	{
		str = ms_doc_construct(document);
		iseof = write(fd, str, ft_strlen(str));
		ms_doc_free(document);
		return (iseof);
	}
	else if(iseof)
		return (0);
	last->content = str;
	last->length = ft_strlen(str);
	if (ms_heredoc(dellimter, fd, last, 1) == -1)
		return (-1);
	str = ms_doc_construct(document);
	iseof = write(fd, str, ft_strlen(str));
	ms_doc_free(document);
	return (iseof);
}
