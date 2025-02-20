/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:26:46 by afelger           #+#    #+#             */
/*   Updated: 2025/02/20 11:27:38 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>

void init_terminal(void)
{
	struct termios term;
	tcgetattr(0, &term);                // Get terminal attributes
	term.c_lflag &= ~ECHOCTL;            // Disable echoing of control characters (like ^C)
	tcsetattr(0, TCSANOW, &term);        // Apply the new terminal attributes
}