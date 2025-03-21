/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 11:12:26 by afelger           #+#    #+#             */
/*   Updated: 2025/02/19 15:24:19 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <stddef.h>
# include <stdlib.h>
# include "libft.h"
# include "ft_printf.h"

# define EMEM_NOT_ALLOC 0xF0

//	Retrieves list of 
t_list	**get_mem(void);

void	*ft_malloc(size_t size);
void	*ft_mem_reg(void *memory);

//this should check if size is inside of sp => then its stack and not freeable
void	ft_free(void *target);

void	cleanup(char reason);

t_list	*ms_lstnew(void *content);


#endif // FT_MALLOC_H