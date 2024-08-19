/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:18 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/08/19 17:17:43 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_env(char **command, char **env)
{
	int	count;

	count = 0;
	if (get_array_len(command) > 1)
	{
		if (command[1][0] == '-')
			return (dprintf(2, ERROR_ENV_OPTION, command[1][1]), 125);
		else
			return (dprintf(2, ERROR_ENV_MANY_ARGS, command[1]), 127);
	}
	while (env[count])
		dprintf(1, "%s\n", env[count++]);
	return (0);
}
