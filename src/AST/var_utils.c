/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 17:57:42 by ckrasniqi         #+#    #+#             */
/*   Updated: 2025/03/27 16:22:44 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit_status_var(char *result, size_t *i)
{
	char	*var_value;

	var_value = ft_itoa(get_appstate()->last_return);
	ft_strlcat(result, var_value, 4 * 1024);
	*i += 1;
	free(var_value);
}
