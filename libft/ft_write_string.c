/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:30:04 by afelger           #+#    #+#             */
/*   Updated: 2024/10/21 09:35:45 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	writeout_string(va_list ap)
{
	int		strl;
	char	*str;

	str = va_arg(ap, char *);
	strl = 0;
	if (str == NULL)
	{
		if (write(1, "(null)", 6) == -1)
			return (-1);
		return (6);
	}
	while (str[strl])
		strl++;
	if (write(1, str, strl) == -1)
		return (-1);
	return (strl);
}
