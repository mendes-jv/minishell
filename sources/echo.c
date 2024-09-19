/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:16 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/08/19 17:17:39 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_option_n(char **command, bool *newline);
static void	print_args(int i, int arr_len, char **command);

int	exec_echo(char **command)
{
	int	arr_len;
	bool newline;

	arr_len = get_array_len(command);
	newline = true;
	print_args(check_option_n(command, &newline), arr_len, command);
	if (newline)
		ft_putstr_fd("\n", 1);
	return (0);
}

static void	print_args(int i, int arr_len, char **command)
{
	while (arr_len > i)
	{
		dprintf(1, "%s", command[i]);
		if (i + 1 != arr_len)
			dprintf(1, " ");
		i++;
	}
	if (i == 0)
		dprintf(1, "\n");
}

static int	check_option_n(char **command, bool *newline)
{
	int	i;
	int	j;

	i = 1;
	j = 2;
	while (command[i] && !ft_strncmp(command[i], "-n", 2))
	{
		j = 2;
		while (command[i][j] == 'n')
			j++;
		if (command[i][j] == '\0')
		{
			i++;
			*newline = false;
		}
		else
			break ;
	}
	return (i);
}
