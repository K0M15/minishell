/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sformat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:56:47 by afelger           #+#    #+#             */
/*   Updated: 2025/02/19 16:28:04 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdlib.h>
#include <stdarg.h>

char *ft_itoa(long long i, char *base);

size_t ft_intstrlen(int i);
size_t ft_uintstrlen(unsigned int i);
size_t ft_ptrstrlen(void *ptr);
size_t ft_hexstrlen(unsigned long i);
size_t 

/**
 * Should create a freeable sting with entered values...
 */
char *sformat(char *fmt, ...)
{
	va_list	ap;
	size_t	count;

	va_start(ap, fmt);
	count = 0;
	while(*fmt)
	{

	}
	va_end(ap);
	va_start(ap);
}