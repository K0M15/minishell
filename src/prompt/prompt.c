/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afelger <afelger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 14:13:02 by afelger           #+#    #+#             */
/*   Updated: 2025/02/18 15:27:57 by afelger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


#define MS_WELC_0 ",--.   ,--,--,--.  ,--,--.,---. ,--.  ,--,------,--.  ,--.   "
#define MS_WELC_1 "|   `.'   |  |  ,'.|  |  '   .-'|  '--'  |  .---|  |  |  |   "
#define MS_WELC_2 "|  |'.'|  |  |  |' '  |  `.  `-.|  .--.  |  `--,|  |  |  |   "
#define MS_WELC_3 "|  |   |  |  |  | `   |  .-'    |  |  |  |  `---|  '--|  '--."
#define MS_WELC_4 "`--'   `--`--`--'  `--`--`-----'`--'  `--`------`-----`-----'"

#define SET_COLOR_VIOLET "\e[48;5;057m"
#define RESET_COLOR "\e[0m"

void	ms_display_welcome(void)
{
	printf("%s%s%s\n", SET_COLOR_VIOLET, MS_WELC_0, RESET_COLOR);
	printf("%s%s%s\n", SET_COLOR_VIOLET, MS_WELC_1, RESET_COLOR);
	printf("%s%s%s\n", SET_COLOR_VIOLET, MS_WELC_2, RESET_COLOR);
	printf("%s%s%s\n", SET_COLOR_VIOLET, MS_WELC_3, RESET_COLOR);
	printf("%s%s%s\n", SET_COLOR_VIOLET, MS_WELC_4, RESET_COLOR);
}

char	*ms_get_prompt(void)
{
	// char *pwd;
	// char *machine;
	// char *user;

	// user = ms_get_env("USER");

	// pwd = get_appstate()->working_directory;
	//get start of last directory. if root + 1 display / in prompt
	return ("minishell >");
}
