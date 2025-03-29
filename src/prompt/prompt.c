/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckrasniq <ckrasniq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 14:13:02 by afelger           #+#    #+#             */
/*   Updated: 2025/03/29 15:08:29 by ckrasniq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <curses.h>


#define MS_WELC_0 "  __  __    ___    _  _     ___     (\")\
     ___             _  _     ___     _       _       "
#define MS_WELC_1 " |  \\/  |  |_ _|  | \\| |   |_ _|     \\|\
    / __|     o O O | || |   | __|   | |     | |      "
#define MS_WELC_2 " | |\\/| |   | |   | .` |    | |            \\__\
 \\    o      | __ |   | _|    | |__   | |__    "
#define MS_WELC_3 " |_|__|_|  |___|  |_|\\_|   |___|   _____   |___/\
   TS__[O] |_||_|   |___|   |____|  |____|   "
#define MS_WELC_4 " |\"\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\
\"\"\"\"|_|      |_|\"\"\"\"\"| {======|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|"
#define MS_WELC_5 " \"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-\
'\"`-0-0-'./o--000'\"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-'"



#define SET_BGCOLOR_ONE "\e[38;5;229m"
#define SET_BGCOLOR_TWO "\e[38;5;228m"
#define SET_BGCOLOR_THREE "\e[38;5;202m"
#define SET_BGCOLOR_FOUR "\e[38;5;196m"
#define SET_BGCOLOR_FIVE "\e[48;5;088m"
#define SET_BGCOLOR_SIX "\e[48;5;052m"
#define RESET_COLOR "\e[0m"



void	ms_display_welcome(void)
{
	printf("%s%s%s\n", SET_BGCOLOR_ONE, MS_WELC_0, RESET_COLOR);
	printf("%s%s%s\n", SET_BGCOLOR_TWO, MS_WELC_1, RESET_COLOR);
	printf("%s%s%s\n", SET_BGCOLOR_THREE, MS_WELC_2, RESET_COLOR);
	printf("%s%s%s\n", SET_BGCOLOR_FOUR, MS_WELC_3, RESET_COLOR);
	printf("%s%s%s\n", SET_BGCOLOR_FIVE, MS_WELC_4, RESET_COLOR);
	printf("%s%s%s\n", SET_BGCOLOR_SIX, MS_WELC_5, RESET_COLOR);

}

char	*ms_get_prompt(void)
{
	if (isatty(STDIN_FILENO))
		return ("minishell> ");
	else
		return ("");
}
