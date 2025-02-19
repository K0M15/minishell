/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 11:12:26 by afelger           #+#    #+#             */
/*   Updated: 2025/02/16 11:20:34 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <stddef.h>
# include "libft.h"

t_list	*get_short();
t_list	*get_long();

void *ft_malloc(size_t size, t_list *target);
void *ft_malloc_short(size_t size);
void *ft_malloc_long(size_t size);

//this should check if size is inside of sp => then its stack and not freeable
void	ft_free(size_t size);

void	cleanup_short();
void	cleanup_long();
void	cleanup_all();

#endif // FT_MALLOC_H