/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:17:17 by afelger           #+#    #+#             */
/*   Updated: 2025/03/23 12:20:11 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_doc_append(t_doc *document, t_doc **last)
{
	while (document->next != NULL)
		document = document->next;
	document->next = malloc(sizeof(t_doc));
	document = document->next;
	if (document == NULL)
		return (-1);
	document->next = NULL;
	*last = document;
	return (0);
}

int	ms_doc_get_length(t_doc *document)
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

char	*ms_doc_construct(t_doc *document)
{
	int		length;
	char	*str;

	length = ms_doc_get_length(document) + 1;
	str = ft_malloc(length);
	if (str == NULL)
		return NULL;
	str[0] = 0;
	while (document != NULL)
	{
		ft_strlcat(str, document->content, length);
		document = document->next;
	}
	return (str);
}

int	ms_doc_display_free(t_doc *document, int fd, int no_var_exp)
{
	char	*str;
	char	*buffer;
	int		written;

	if (no_var_exp)
		str = ms_doc_construct(document);
	else
	{
		buffer = ms_doc_construct(document);
		// str = expand_variables_in_string(buffer);
		ft_free(buffer);
	}
	written = write(fd, str, ft_strlen(str));
	ms_doc_free(document);
	ft_free(str);
	return (written);
}

t_doc	*ms_doc_app_or_new(t_doc **document)
{
	t_doc *new;

	if (*document == NULL)
	{
		new = malloc(sizeof(t_doc));
		if (new == NULL)
			return NULL;
		new->next = NULL;
	}
	else if(ms_doc_append(*document, &new) == -1)
		return NULL;
	return (new);
}
