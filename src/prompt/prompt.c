/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 14:13:02 by afelger           #+#    #+#             */
/*   Updated: 2025/03/04 16:23:15 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <curses.h>


#define MS_WELC_0 ",--.   ,--,--,--.  ,--,--.,---. ,--.  ,--,------,--.  ,--.   "
#define MS_WELC_1 "|   `.'   |  |  ,'.|  |  '   .-'|  '--'  |  .---|  |  |  |   "
#define MS_WELC_2 "|  |'.'|  |  |  |' '  |  `.  `-.|  .--.  |  `--,|  |  |  |   "
#define MS_WELC_3 "|  |   |  |  |  | `   |  .-'    |  |  |  |  `---|  '--|  '--."
#define MS_WELC_4 "`--'   `--`--`--'  `--`--`-----'`--'  `--`------`-----`-----'"

#define SET_BGCOLOR_ONE "\e[48;5;058m"
#define SET_BGCOLOR_TWO "\e[48;5;059m"
#define SET_BGCOLOR_THREE "\e[48;5;060m"
#define SET_BGCOLOR_FOUR "\e[48;5;061m"
#define SET_BGCOLOR_FIVE "\e[48;5;062m"
#define RESET_COLOR "\e[0m"

void	ms_display_welcome(void)
{
	printf("%s%s%s\n", SET_BGCOLOR_ONE, MS_WELC_0, RESET_COLOR);
	printf("%s%s%s\n", SET_BGCOLOR_TWO, MS_WELC_1, RESET_COLOR);
	printf("%s%s%s\n", SET_BGCOLOR_THREE, MS_WELC_2, RESET_COLOR);
	printf("%s%s%s\n", SET_BGCOLOR_FOUR, MS_WELC_3, RESET_COLOR);
	printf("%s%s%s\n", SET_BGCOLOR_FIVE, MS_WELC_4, RESET_COLOR);
}

char	*ms_get_prompt(void)
{
	if (isatty(STDIN_FILENO))
		return ("minishell >");
	else
		return ("");
}
