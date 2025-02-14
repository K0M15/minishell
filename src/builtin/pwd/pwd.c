/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:08:13 by afelger           #+#    #+#             */
/*   Updated: 2025/02/14 10:43:43 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/param.h>
#include <stdlib.h>
#include "ft_printf.h"

int pwd(int argc, char **argv)
{
	char *buffer;
	
	buffer = getenv("PWD");
	if(buffer == NULL)
		return (-1);
	ft_printf("%s\n", buffer);
}