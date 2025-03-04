/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dyn_str.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 12:33:29 by afelger           #+#    #+#             */
/*   Updated: 2025/03/04 14:08:46 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DYN_STR_H
# define FT_DYN_STR_H
# include "libft.h"
# include "stdlib.h"
# define DYN_STR_ALLOC	50

typedef struct s_dyn_str
{
	char	*str;
	size_t	alloc;
	size_t	filled;
}	t_dyn_str;

t_dyn_str	*dyn_str_new(void);
int			dyn_str_enhance(t_dyn_str *str);
int			dyn_str_addchar(t_dyn_str *str, char c);
int			dyn_str_addstr(t_dyn_str *str, char *src);
void		dyn_str_free(t_dyn_str *str);

#endif // FT_DYN_STR_H